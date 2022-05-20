#ifndef BURNERPROGRESSBARSSETUP_H
#define BURNERPROGRESSBARSSETUP_H

#include "burnerwidgets.h"

#include <QProgressBar>


class BurnerProgressBarsSetup
{
public:
    BurnerProgressBarsSetup(const BurnerProgressBarsWidgets& new_burner_progress_bars_widgets);
    void setValuesProgressBarsToZero();
    void resetBurnerProgressBarValues();
    void resetRingAndUnitProgressBarsValues();
    void restoreBurnerProgressBarValues();
    void restoreRingAndUnitProgressBarsValues();

private:
    BurnerProgressBarsWidgets burner_progress_bars_widgets;
};

#endif // BURNERPROGRESSBARSSETUP_H
