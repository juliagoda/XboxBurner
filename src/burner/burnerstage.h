#ifndef BURNERSTAGE_H
#define BURNERSTAGE_H

#include "burnerprogressbarssetup.h"
#include "burnerwidgets.h"

#include <QString>
#include <QProcess>


class BurnerStage : public QObject
{
    Q_OBJECT

public:
    BurnerStage(const BurnerWidgets& new_burner_widgets) :
        burner_widgets{new_burner_widgets},
        burner_progress_bars_setup{{new_burner_widgets.progress_bar_burn,
                                   new_burner_widgets.progress_bar_ring_buffer_unit,
                                   new_burner_widgets.progress_bar_unit_buffer_unit}},
        next_stage{nullptr},
        burn_process{nullptr},
        burning{false}
    {

    }

    BurnerStage* next(BurnerStage* next_burner_stage)
    {
        next_stage = next_burner_stage;
        return next_stage;
    }

    virtual bool handle()
    {
        if (next_stage)
            return next_stage->handle();

        return true;
    }

protected:
    BurnerWidgets burner_widgets;
    BurnerProgressBarsSetup burner_progress_bars_setup;
    BurnerStage* next_stage;
    QProcess* burn_process;
    bool burning;
};

#endif // BURNERSTAGE_H
