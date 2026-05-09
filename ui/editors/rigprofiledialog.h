// ui/editors/rigprofiledialog.h
//
// GUI for managing rig profiles. A rig profile maps symbolic role names
// ("Cell_A", "Cell_B", …) to the physical AI/AO channels, gains, and
// current limits for one rig. Profiles live as .rigprofile.json files
// under the user's application config directory; one of them is the
// "active" profile at any time, used to resolve biological-cell roles
// at network-load time.
//
// See User_Guide.md §Rig Profiles for the user-level workflow.

#ifndef RIGPROFILEDIALOG_H
#define RIGPROFILEDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>
#include <QSettings>
#include <QInputDialog>
#include <QDialogButtonBox>
#include <QCheckBox>

#include <memory>

#include "rig_profile.h"
#include "daq_enumerate.h"

class RigProfileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RigProfileDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Rig Profiles");
        resize(860, 520);

        auto *main = new QHBoxLayout(this);

        // === Left: list of profiles ===
        auto *leftBox = new QVBoxLayout;
        leftBox->addWidget(new QLabel("Profiles"));
        m_profileList = new QListWidget;
        leftBox->addWidget(m_profileList);

        auto *listBtnRow = new QHBoxLayout;
        auto *newBtn       = new QPushButton("New");
        auto *dupBtn       = new QPushButton("Duplicate");
        auto *delBtn       = new QPushButton("Delete");
        listBtnRow->addWidget(newBtn);
        listBtnRow->addWidget(dupBtn);
        listBtnRow->addWidget(delBtn);
        leftBox->addLayout(listBtnRow);

        m_setActiveBtn = new QPushButton("Set as Active Profile");
        leftBox->addWidget(m_setActiveBtn);
        m_activeLabel = new QLabel("Active: (none)");
        m_activeLabel->setStyleSheet("color: #0a7a30; font-weight: bold;");
        leftBox->addWidget(m_activeLabel);

        auto *leftW = new QWidget; leftW->setLayout(leftBox);
        leftW->setMaximumWidth(260);
        main->addWidget(leftW);

        // === Right: editor ===
        auto *rightBox = new QVBoxLayout;
        auto *hdr = new QFormLayout;
        m_nameEdit = new QLineEdit;
        hdr->addRow("Name:", m_nameEdit);
        m_descEdit = new QLineEdit;
        hdr->addRow("Description:", m_descEdit);
        rightBox->addLayout(hdr);

        rightBox->addWidget(new QLabel("Role bindings"));
        m_bindingsTable = new QTableWidget(0, 7);
        QStringList headers = {"Role", "AI Channel", "AO Channel",
                               "AI Gain (mV/V)", "AO Gain (V/nA)",
                               "I+ limit (nA)", "I− limit (nA)"};
        m_bindingsTable->setHorizontalHeaderLabels(headers);
        m_bindingsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        rightBox->addWidget(m_bindingsTable, 1);

        auto *bindBtnRow = new QHBoxLayout;
        auto *addRowBtn = new QPushButton("+ Add binding");
        auto *delRowBtn = new QPushButton("− Remove selected");
        bindBtnRow->addWidget(addRowBtn);
        bindBtnRow->addWidget(delRowBtn);
        bindBtnRow->addStretch();
        rightBox->addLayout(bindBtnRow);

        // Hint for users
        auto *hint = new QLabel(
            "<i>Tip: AI channel dropdowns are populated from the NI-DAQmx "
            "driver. Role names are symbolic — anything you'll reference "
            "from a network file.</i>");
        hint->setWordWrap(true);
        rightBox->addWidget(hint);

        auto *btnBox = new QDialogButtonBox(
            QDialogButtonBox::Save | QDialogButtonBox::Close);
        rightBox->addWidget(btnBox);

        main->addLayout(rightBox, 1);

        // --- Wiring ---
        connect(m_profileList, &QListWidget::currentRowChanged,
                this, &RigProfileDialog::onProfileSelected);
        connect(newBtn, &QPushButton::clicked, this, &RigProfileDialog::onNewProfile);
        connect(dupBtn, &QPushButton::clicked, this, &RigProfileDialog::onDuplicate);
        connect(delBtn, &QPushButton::clicked, this, &RigProfileDialog::onDelete);
        connect(m_setActiveBtn, &QPushButton::clicked,
                this, &RigProfileDialog::onSetActive);
        connect(addRowBtn, &QPushButton::clicked, this, &RigProfileDialog::onAddBinding);
        connect(delRowBtn, &QPushButton::clicked, this, &RigProfileDialog::onRemoveBinding);
        connect(btnBox->button(QDialogButtonBox::Save), &QPushButton::clicked,
                this, &RigProfileDialog::onSave);
        connect(btnBox->button(QDialogButtonBox::Close), &QPushButton::clicked,
                this, &QDialog::accept);

        // Populate channel hints once — used when adding bindings.
        populateChannelSuggestions();

        // Load profiles from disk.
        refreshProfileList();
        updateActiveLabel();
    }

    /// Directory where profile files live. Created if missing.
    static QString profilesDir() {
        QString base = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        if (base.isEmpty()) base = QDir::homePath() + "/.netrex";
        QDir().mkpath(base + "/rig_profiles");
        return base + "/rig_profiles";
    }

    /// Load the profile named in QSettings["activeProfile"], set it active.
    static void restoreActiveFromSettings() {
        QSettings s("NETrex", "NETrex");
        QString name = s.value("activeProfile").toString();
        if (name.isEmpty()) return;
        QString path = profilesDir() + "/" + name + ".rigprofile.json";
        auto p = std::make_shared<RigProfile>();
        std::string err;
        if (rig_profile_json::load(path.toStdString(), *p, &err)) {
            RigProfileManager::instance().set_active(p);
        }
    }

private slots:
    void onProfileSelected(int row) {
        if (row < 0 || row >= static_cast<int>(m_loaded.size())) {
            clearEditor();
            return;
        }
        loadIntoEditor(m_loaded[row]);
    }

    void onNewProfile() {
        bool ok = false;
        QString name = QInputDialog::getText(this, "New Rig Profile",
            "Profile name:", QLineEdit::Normal, "", &ok);
        if (!ok || name.isEmpty()) return;
        if (profileExists(name)) {
            QMessageBox::warning(this, "Exists", "A profile with that name already exists.");
            return;
        }

        RigProfile p;
        p.name = name.toStdString();
        if (!saveProfileFile(p)) return;
        refreshProfileList();
        selectProfile(name);
    }

    void onDuplicate() {
        int row = m_profileList->currentRow();
        if (row < 0) return;
        RigProfile src = m_loaded[row];

        bool ok = false;
        QString newName = QInputDialog::getText(this, "Duplicate Profile",
            "New name:", QLineEdit::Normal,
            QString::fromStdString(src.name) + "_copy", &ok);
        if (!ok || newName.isEmpty()) return;
        if (profileExists(newName)) {
            QMessageBox::warning(this, "Exists", "A profile with that name already exists.");
            return;
        }
        src.name = newName.toStdString();
        if (!saveProfileFile(src)) return;
        refreshProfileList();
        selectProfile(newName);
    }

    void onDelete() {
        int row = m_profileList->currentRow();
        if (row < 0) return;
        QString name = QString::fromStdString(m_loaded[row].name);
        if (QMessageBox::question(this, "Delete",
                QString("Delete profile \"%1\"?").arg(name)) != QMessageBox::Yes)
            return;
        QFile::remove(profileFilePath(name));
        // If it was active, clear.
        QSettings s("NETrex", "NETrex");
        if (s.value("activeProfile").toString() == name) {
            s.remove("activeProfile");
            RigProfileManager::instance().set_active(nullptr);
        }
        refreshProfileList();
        updateActiveLabel();
    }

    void onSetActive() {
        int row = m_profileList->currentRow();
        if (row < 0) return;
        RigProfile p = m_loaded[row];

        // Commit pending edits first — Set Active on a dirty editor was
        // surprising in testing.
        if (!commitEditorToProfile(p)) return;
        if (!saveProfileFile(p)) return;
        m_loaded[row] = p;

        auto shared = std::make_shared<RigProfile>(p);
        RigProfileManager::instance().set_active(shared);

        QSettings s("NETrex", "NETrex");
        s.setValue("activeProfile", QString::fromStdString(p.name));

        updateActiveLabel();
    }

    void onAddBinding() {
        int row = m_bindingsTable->rowCount();
        m_bindingsTable->insertRow(row);
        // Reasonable defaults.
        m_bindingsTable->setItem(row, 0, new QTableWidgetItem(
            QString("Cell_%1").arg(QChar('A' + row))));
        m_bindingsTable->setItem(row, 1, new QTableWidgetItem(
            m_firstAi.isEmpty() ? "Dev1/ai0" : m_firstAi));
        m_bindingsTable->setItem(row, 2, new QTableWidgetItem(
            m_firstAo.isEmpty() ? "Dev1/ao0" : m_firstAo));
        m_bindingsTable->setItem(row, 3, new QTableWidgetItem("10"));
        m_bindingsTable->setItem(row, 4, new QTableWidgetItem("1"));
        m_bindingsTable->setItem(row, 5, new QTableWidgetItem("5"));
        m_bindingsTable->setItem(row, 6, new QTableWidgetItem("-5"));
    }

    void onRemoveBinding() {
        int row = m_bindingsTable->currentRow();
        if (row >= 0) m_bindingsTable->removeRow(row);
    }

    void onSave() {
        int row = m_profileList->currentRow();
        if (row < 0) {
            QMessageBox::information(this, "Save", "No profile selected.");
            return;
        }
        RigProfile p = m_loaded[row];
        if (!commitEditorToProfile(p)) return;
        // If the name was edited, the on-disk filename must change too.
        QString oldName = QString::fromStdString(m_loaded[row].name);
        QString newName = QString::fromStdString(p.name);
        if (newName != oldName) {
            if (profileExists(newName)) {
                QMessageBox::warning(this, "Save",
                    "A profile with that name already exists.");
                return;
            }
            QFile::remove(profileFilePath(oldName));
            // If the renamed one was active, update QSettings.
            QSettings s("NETrex", "NETrex");
            if (s.value("activeProfile").toString() == oldName) {
                s.setValue("activeProfile", newName);
            }
        }
        if (!saveProfileFile(p)) return;
        m_loaded[row] = p;
        refreshProfileList();
        selectProfile(QString::fromStdString(p.name));
        updateActiveLabel();
    }

private:
    void populateChannelSuggestions() {
        auto devs = enumerate_available_devices();
        for (const auto &dev : devs) {
            for (const auto &ch : dev.aiChannels) {
                QString q = QString::fromStdString(ch);
                if (m_firstAi.isEmpty()) m_firstAi = q;
            }
            for (const auto &ch : dev.aoChannels) {
                QString q = QString::fromStdString(ch);
                if (m_firstAo.isEmpty()) m_firstAo = q;
            }
        }
    }

    QString profileFilePath(const QString &name) const {
        return profilesDir() + "/" + name + ".rigprofile.json";
    }

    bool profileExists(const QString &name) const {
        return QFile::exists(profileFilePath(name));
    }

    bool saveProfileFile(const RigProfile &p) {
        std::string err;
        if (!rig_profile_json::save(
                p, profileFilePath(QString::fromStdString(p.name)).toStdString(),
                &err)) {
            QMessageBox::warning(this, "Save",
                QString::fromStdString("Save failed: " + err));
            return false;
        }
        return true;
    }

    void refreshProfileList() {
        m_loaded.clear();
        m_profileList->clear();
        QDir dir(profilesDir());
        auto files = dir.entryList(QStringList() << "*.rigprofile.json",
                                   QDir::Files, QDir::Name);
        for (const QString &fn : files) {
            RigProfile p;
            std::string err;
            if (rig_profile_json::load(
                    (profilesDir() + "/" + fn).toStdString(), p, &err)) {
                m_loaded.push_back(p);
                m_profileList->addItem(QString::fromStdString(p.name));
            }
        }
        clearEditor();
    }

    void selectProfile(const QString &name) {
        auto items = m_profileList->findItems(name, Qt::MatchExactly);
        if (!items.isEmpty()) m_profileList->setCurrentItem(items.first());
    }

    void clearEditor() {
        m_nameEdit->clear();
        m_descEdit->clear();
        m_bindingsTable->setRowCount(0);
    }

    void loadIntoEditor(const RigProfile &p) {
        m_nameEdit->setText(QString::fromStdString(p.name));
        m_descEdit->setText(QString::fromStdString(p.description));
        m_bindingsTable->setRowCount(static_cast<int>(p.bindings.size()));
        for (int i = 0; i < static_cast<int>(p.bindings.size()); ++i) {
            const auto &b = p.bindings[i];
            m_bindingsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(b.role)));
            m_bindingsTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(b.ai_channel)));
            m_bindingsTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(b.ao_channel)));
            m_bindingsTable->setItem(i, 3, new QTableWidgetItem(QString::number(b.ai_gain)));
            m_bindingsTable->setItem(i, 4, new QTableWidgetItem(QString::number(b.ao_gain)));
            m_bindingsTable->setItem(i, 5, new QTableWidgetItem(QString::number(b.pos_limit_nA)));
            m_bindingsTable->setItem(i, 6, new QTableWidgetItem(QString::number(b.neg_limit_nA)));
        }
    }

    bool commitEditorToProfile(RigProfile &p) {
        QString name = m_nameEdit->text().trimmed();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "Save", "Profile name cannot be empty.");
            return false;
        }
        p.name = name.toStdString();
        p.description = m_descEdit->text().trimmed().toStdString();
        p.bindings.clear();
        for (int i = 0; i < m_bindingsTable->rowCount(); ++i) {
            RoleBinding b;
            auto get = [&](int col) -> QString {
                auto *it = m_bindingsTable->item(i, col);
                return it ? it->text() : QString();
            };
            b.role       = get(0).trimmed().toStdString();
            b.ai_channel = get(1).trimmed().toStdString();
            b.ao_channel = get(2).trimmed().toStdString();
            bool ok = true, ok2 = true;
            b.ai_gain      = get(3).toDouble(&ok);
            b.ao_gain      = get(4).toDouble(&ok2);
            b.pos_limit_nA = get(5).toDouble();
            b.neg_limit_nA = get(6).toDouble();
            if (b.role.empty()) {
                QMessageBox::warning(this, "Save",
                    QString("Row %1: role name is required.").arg(i + 1));
                return false;
            }
            if (!ok || !ok2) {
                QMessageBox::warning(this, "Save",
                    QString("Row %1: invalid AI or AO gain.").arg(i + 1));
                return false;
            }
            p.bindings.push_back(b);
        }
        // Reject duplicate role names (they'd silently shadow each other).
        for (std::size_t i = 0; i < p.bindings.size(); ++i) {
            for (std::size_t j = i + 1; j < p.bindings.size(); ++j) {
                if (p.bindings[i].role == p.bindings[j].role) {
                    QMessageBox::warning(this, "Save",
                        QString("Duplicate role \"%1\".").arg(
                            QString::fromStdString(p.bindings[i].role)));
                    return false;
                }
            }
        }
        return true;
    }

    void updateActiveLabel() {
        QSettings s("NETrex", "NETrex");
        QString active = s.value("activeProfile").toString();
        m_activeLabel->setText(active.isEmpty()
            ? "Active: (none)"
            : QString("Active: %1").arg(active));
    }

    QListWidget   *m_profileList;
    QLineEdit     *m_nameEdit;
    QLineEdit     *m_descEdit;
    QTableWidget  *m_bindingsTable;
    QPushButton   *m_setActiveBtn;
    QLabel        *m_activeLabel;
    std::vector<RigProfile> m_loaded;
    QString        m_firstAi, m_firstAo;   // channel-suggestion defaults
};

#endif  // RIGPROFILEDIALOG_H
