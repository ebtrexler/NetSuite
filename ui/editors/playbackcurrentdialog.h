#ifndef PLAYBACKCURRENTDIALOG_H
#define PLAYBACKCURRENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include "RT_PlaybackCurrent.h"
#include "playbackwaveformdialog.h"

class PlaybackCurrentDialog : public QDialog
{
    Q_OBJECT
public:
    PlaybackCurrentDialog(TPlaybackCurrent *cur, QWidget *parent = nullptr)
        : QDialog(parent), m_cur(cur)
    {
        setWindowTitle(QString("Playback Current: %1").arg(QString::fromStdWString(cur->Name())));
        auto *layout = new QVBoxLayout(this);

        auto *form = new QFormLayout();
        gmaxEdit = new QLineEdit(QString::number(cur->Gmax()));
        eEdit = new QLineEdit(QString::number(cur->E()));
        gnoiseEdit = new QLineEdit(QString::number(cur->Gnoise()));
        form->addRow("Gmax (µS):", gmaxEdit);
        form->addRow("E (mV):", eEdit);
        form->addRow("Gnoise (%):", gnoiseEdit);
        layout->addLayout(form);

        wfWidget = new PlaybackWaveformWidget(&cur->Waveform(), "conductance", this);
        layout->addWidget(wfWidget);

        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttons, &QDialogButtonBox::accepted, this, &PlaybackCurrentDialog::tryAccept);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        layout->addWidget(buttons);
    }

private slots:
    void tryAccept() {
        bool ok;
        double gmax = gmaxEdit->text().toDouble(&ok); if (!ok) { QMessageBox::warning(this, "Error", "Invalid Gmax"); return; }
        double e = eEdit->text().toDouble(&ok); if (!ok) { QMessageBox::warning(this, "Error", "Invalid E"); return; }
        double gn = gnoiseEdit->text().toDouble(&ok); if (!ok) { QMessageBox::warning(this, "Error", "Invalid Gnoise"); return; }
        if (!wfWidget->applyToWaveform()) return;
        m_cur->Gmax(gmax); m_cur->E(e); m_cur->Gnoise(gn);
        accept();
    }

private:
    TPlaybackCurrent *m_cur;
    QLineEdit *gmaxEdit, *eEdit, *gnoiseEdit;
    PlaybackWaveformWidget *wfWidget;
};

#endif
