#ifndef BURNERPROGRESSBARSSETUP_H
#define BURNERPROGRESSBARSSETUP_H

#include <QProgressBar>

struct BurnerProgressBarsWidgets
{
    QProgressBar* progress_bar_burn = nullptr;
    QProgressBar* progress_bar_ring_buffer_unit = nullptr;
    QProgressBar* progress_bar_unit_buffer_unit = nullptr;
};

class BurnerProgressBarsSetup
{
public:
    BurnerProgressBarsSetup(const BurnerProgressBarsWidgets& new_burner_progress_bars_widgets);
    void resetBurnerProgressBarValues();
    void resetRingAndUnitProgressBarsValues();
    void restoreBurnerProgressBarValues();
    void restoreRingAndUnitProgressBarsValues();

private:
    BurnerProgressBarsWidgets burner_progress_bars_widgets;
};

#endif // BURNERPROGRESSBARSSETUP_H
