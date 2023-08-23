import QtQuick                      2.3
import QtQuick.Controls             1.2
import QtQuick.Layouts              1.2

import QGroundControl               1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Controls      1.0
import QGroundControl.FactSystem    1.0
import QGroundControl.FactControls  1.0

ColumnLayout {
    spacing: _margin
    visible: tabBar.currentIndex === 4

    property var missionItem

    MouseArea {
        Layout.preferredWidth:  childrenRect.width
        Layout.preferredHeight: childrenRect.height

        onClicked: {
            var removeModes = []
            var updateFunction = function(seMode){ missionItem.steelEagleMode = seMode }
            // removeModes.push(QGroundControl.AltitudeModeMixed)
            // if (!missionItem.masterController.controllerVehicle.supportsTerrainFrame) {
            //     removeModes.push(QGroundControl.AltitudeModeTerrainFrame)
            // }
            // if (!QGroundControl.corePlugin.options.showMissionAbsoluteAltitude || !_missionItem.cameraCalc.isManualCamera) {
            //     removeModes.push(QGroundControl.AltitudeModeAbsolute)
            // }
            steelEagleModeDialogComponent.createObject(mainWindow, { rgRemoveModes: removeModes, updateSEModeFn: updateFunction }).open()
        }

        Component { id: steelEagleModeDialogComponent; SteelEagleModeDialog { } }

        RowLayout {
            spacing: ScreenTools.defaultFontPixelWidth / 2

            QGCLabel { text: QGroundControl.steelEagleModeShortDescription(missionItem.steelEagleMode) }
            QGCColoredImage {
                height:     ScreenTools.defaultFontPixelHeight / 2
                width:      height
                source:     "/res/DropArrow.svg"
                color:      qgcPal.text
            }
        }
    }



    // detect
    GridLayout {
        Layout.fillWidth:   true
        columnSpacing:      _margin
        rowSpacing:         _margin
        columns:            2
        enabled:            missionItem.steelEagleMode === QGroundControl.DetectTask

        QGCLabel { text: qsTr("gimbal_pitch") }
        // FactTextField {
        //     fact:               missionItem.detectTask_gimbal_pitch
        //     Layout.fillWidth:   true
        // }

        QGCLabel { text: qsTr("drone_rotation") }
        // FactTextField {
        //     fact:               missionItem.detectTask_drone_rotation
        //     Layout.fillWidth:   true
        // }

        QGCLabel { text: qsTr("sample_rate") }
        // FactTextField {
        //     fact:               missionItem.detectTask_sample_rate
        //     Layout.fillWidth:   true
        // }

        QGCLabel { text: qsTr("hover_delay") }
        // FactTextField {
        //     fact:               missionItem.detectTask_hover_delay
        //     Layout.fillWidth:   true
        // }
        
        QGCLabel { text: qsTr("model") }
        // QGCComboBox {
            
        //     id:                 detectTaskModelCombo
        //     Layout.fillWidth:   true
        
        //     model:              missionItem.detectTask_modellist

        //     onActivated:        missionItem.detectTask_model_2 = currentText
        // }
    }

    // obstacle
    GridLayout {
        Layout.fillWidth:   true
        columnSpacing:      _margin
        rowSpacing:         _margin
        columns:            2
        enabled:            missionItem.steelEagleMode === QGroundControl.ObstacleTask

        QGCLabel { text: qsTr("model") }
        // FactTextField {
        //     fact:               missionItem.obstacleTask_model
        //     Layout.fillWidth:   true
        // }

        QGCLabel { text: qsTr("speed") }
        // FactTextField {
        //     fact:               missionItem.obstacleTask_speed
        //     Layout.fillWidth:   true
        // }

        QGCLabel { text: qsTr("altitude") }
        // FactTextField {
        //     fact:               missionItem.obstacleTask_altitude
        //     Layout.fillWidth:   true
        // }
    }

    // tracking
    GridLayout {
        Layout.fillWidth:   true
        columnSpacing:      _margin
        rowSpacing:         _margin
        columns:            2
        enabled:            missionItem.steelEagleMode === QGroundControl.TrackingTask

        QGCLabel { text: qsTr("gimbal_pitch") }
        // FactTextField {
        //     fact:               missionItem.trackingTask_gimbal_pitch
        //     Layout.fillWidth:   true
        // }

                
        QGCLabel { text: qsTr("model") }
        // QGCComboBox {
            
        //     id:                 trackingTaskModelCombo
        //     Layout.fillWidth:   true
        
        //     model:              missionItem.trackingTask_modellist

        //     onActivated:        missionItem.trackingTask_model_2 = currentText
        // }

        QGCLabel { text: qsTr("class") }
        // FactTextField {
        //     fact:               missionItem.trackingTask_class
        //     Layout.fillWidth:   true
        // }
    }
}
