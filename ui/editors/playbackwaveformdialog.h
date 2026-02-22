#ifndef PLAYBACKWAVEFORMDIALOG_H
#define PLAYBACKWAVEFORMDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QMessageBox>
#include "RT_PlaybackWaveform.h"

/// Reusable widget for editing TPlaybackWaveform parameters (embedded in other dialogs)
class PlaybackWaveformWidget : public QWidget
{
    Q_OBJECT
public:
    PlaybackWaveformWidget(TPlaybackWaveform *wf, const QString &units, QWidget *parent = nullptr)
        : QWidget(parent), m_wf(wf), m_units(units)
    {
        auto *layout = new QVBoxLayout(this);

        // File selection
        auto *fileLayout = new QHBoxLayout();
        fileLabel = new QLabel(QString::fromStdString(wf->FileName().empty() ? "(no file)" : wf->FileName()));
        auto *browseBtn = new QPushButton("Browse...");
        connect(browseBtn, &QPushButton::clicked, this, &PlaybackWaveformWidget::browse);
        fileLayout->addWidget(fileLabel, 1);
        fileLayout->addWidget(browseBtn);
        layout->addLayout(fileLayout);

        // File options
        auto *optGroup = new QGroupBox("File Options");
        auto *optForm = new QFormLayout(optGroup);
        timeColCheck = new QCheckBox("First column is time");
        timeColCheck->setChecked(wf->TXTTimeColumn());
        headerCheck = new QCheckBox("First row is header");
        headerCheck->setChecked(wf->TXTHeaderRow());
        channelCombo = new QComboBox();
        for (auto &n : wf->ChannelNames()) channelCombo->addItem(QString::fromStdString(n));
        if (wf->SelectedChannel() >= 0 && wf->SelectedChannel() < channelCombo->count())
            channelCombo->setCurrentIndex(wf->SelectedChannel());
        optForm->addRow(timeColCheck);
        optForm->addRow(headerCheck);
        optForm->addRow("Channel:", channelCombo);
        layout->addWidget(optGroup);

        // Waveform params
        auto *paramGroup = new QGroupBox("Waveform Parameters");
        auto *paramForm = new QFormLayout(paramGroup);
        scaleEdit = new QLineEdit(QString::number(wf->ScaleFactor()));
        offsetEdit = new QLineEdit(QString::number(wf->Offset()));
        rateEdit = new QLineEdit(QString::number(wf->PlaybackRate()));
        durationLabel = new QLabel(QString::number(wf->FileDuration()) + " ms");
        repeatSpin = new QSpinBox();
        repeatSpin->setRange(-1, 100000);
        repeatSpin->setValue(wf->RepeatNumber());
        repeatSpin->setSpecialValueText("forever");
        delayEdit = new QLineEdit(QString::number(wf->DelayBeforeRepeat()));
        paramForm->addRow("Scale factor:", scaleEdit);
        paramForm->addRow("Offset (" + units + "):", offsetEdit);
        paramForm->addRow("Sample rate (ms/pt):", rateEdit);
        paramForm->addRow("File duration:", durationLabel);
        paramForm->addRow("Repeats (-1=forever):", repeatSpin);
        paramForm->addRow("Delay between repeats (ms):", delayEdit);
        layout->addWidget(paramGroup);
    }

    bool applyToWaveform() {
        bool ok;
        double scale = scaleEdit->text().toDouble(&ok);
        if (!ok) { QMessageBox::warning(this, "Error", "Invalid scale factor"); return false; }
        double offset = offsetEdit->text().toDouble(&ok);
        if (!ok) { QMessageBox::warning(this, "Error", "Invalid offset"); return false; }
        double rate = rateEdit->text().toDouble(&ok);
        if (!ok || rate <= 0) { QMessageBox::warning(this, "Error", "Rate must be > 0"); return false; }
        double delay = delayEdit->text().toDouble(&ok);
        if (!ok || delay < 0) { QMessageBox::warning(this, "Error", "Delay must be >= 0"); return false; }

        m_wf->SetScaleFactor(scale);
        m_wf->SetOffset(offset);
        m_wf->SetPlaybackRate(rate);
        m_wf->SetRepeatNumber(repeatSpin->value());
        m_wf->SetDelayBeforeRepeat(delay);
        m_wf->SetTXTTimeColumn(timeColCheck->isChecked());
        m_wf->SetTXTHeaderRow(headerCheck->isChecked());
        if (channelCombo->currentIndex() >= 0)
            m_wf->SetSelectedChannel(channelCombo->currentIndex());
        return true;
    }

private slots:
    void browse() {
        QString file = QFileDialog::getOpenFileName(this, "Open Waveform File", "",
            "Text files (*.csv *.txt *.dat);;All files (*)");
        if (file.isEmpty()) return;
        m_wf->SetTXTTimeColumn(timeColCheck->isChecked());
        m_wf->SetTXTHeaderRow(headerCheck->isChecked());
        m_wf->SetFileName(file.toStdString());
        fileLabel->setText(file);
        durationLabel->setText(QString::number(m_wf->FileDuration()) + " ms");
        channelCombo->clear();
        for (auto &n : m_wf->ChannelNames()) channelCombo->addItem(QString::fromStdString(n));
    }

private:
    TPlaybackWaveform *m_wf;
    QString m_units;
    QLabel *fileLabel, *durationLabel;
    QCheckBox *timeColCheck, *headerCheck;
    QComboBox *channelCombo;
    QLineEdit *scaleEdit, *offsetEdit, *rateEdit, *delayEdit;
    QSpinBox *repeatSpin;
};

#endif
