import QtQuick                      2.3

import QGroundControl               1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Controls      1.0

QGCTabBar {
    id: tabBar

    Component.onCompleted: currentIndex = QGroundControl.settingsManager.planViewSettings.displayPresetsTabFirst.rawValue ? 2 : 0

    QGCTabButton { icon.source: "/qmlimages/PatternGrid.png"; icon.height: ScreenTools.defaultFontPixelHeight }
    QGCTabButton { icon.source: "/qmlimages/PatternScotty.png"; icon.height: ScreenTools.defaultFontPixelHeight }
}
