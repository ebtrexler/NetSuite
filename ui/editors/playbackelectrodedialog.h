#ifndef PLAYBACKELECTRODEDIALOG_H
#define PLAYBACKELECTRODEDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include "RT_PlaybackElectrode.h"
#include "playbackwaveformdialog.h"

class PlaybackElectrodeDialog : public QDialog
{
    Q_OBJECT
public:
    PlaybackElectrodeDialog(TPlaybackElectrode *el, QWidget *parent = nullptr)
        : QDialog(parent), m_el(el)
    {
        setWindowTitle(QString("Playback Electrode: %1").arg(QString::fromStdWString(el->Name())));
        auto *layout = new QVBoxLayout(this);

        wfWidget = new PlaybackWaveformWidget(&el->Waveform(), "nA", this);
        layout->addWidget(wfWidget);

        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttons, &QDialogButtonBox::accepted, this, &PlaybackElectrodeDialog::tryAccept);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        layout->addWidget(buttons);
    }

private slots:
    void tryAccept() {
        if (!wfWidget->applyToWaveform()) return;
        accept();
    }

private:
    TPlaybackElectrode *m_el;
    PlaybackWaveformWidget *wfWidget;
};

#endif
