/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick          2.3
import QtQuick.Controls 2.12
import QtQuick.Dialogs  1.3
import QtQuick.Layouts  1.12

import QGroundControl               1.0
import QGroundControl.Controls      1.0
import QGroundControl.ScreenTools   1.0

QGCPopupDialog {
    title:   qsTr("Steel Eagle Mode")
    buttons: StandardButton.Close

    property var rgRemoveModes
    property var updateSEModeFn
    property var currentSEMode

    Component.onCompleted: {
        // Check for custom build override on AMSL usage
        // if (!QGroundControl.corePlugin.options.showMissionAbsoluteAltitude && currentSEMode != QGroundControl.AltitudeModeAbsolute) {
        //     rgRemoveModes.push(QGroundControl.AltitudeModeAbsolute)
        // }

        // Remove modes specified by consumer
        // for (var i=0; i<rgRemoveModes.length; i++) {
        //     for (var j=0; j<buttonModel.count; j++) {
        //         if (buttonModel.get(j).modeValue == rgRemoveModes[i]) {
        //             buttonModel.remove(j)
        //             break
        //         }
        //     }
        // }


        buttonRepeater.model = buttonModel
    }

    ListModel {
        id: buttonModel

        ListElement {
            modeName:   qsTr("Detect Task")
            help:       qsTr("Instruct drone to detect objects along the specified path using the specified pitch, rotation, sampling rate, and detection model ")
            modeValue:  QGroundControl.DetectTask
        }

        ListElement {
            modeName:   qsTr("Obstacle Task")
            help:       qsTr("Fly from coordinate A to coordinate B while avoiding obstacles")
            modeValue:  QGroundControl.ObstacleTask
        }
        ListElement {
            modeName:   qsTr("Tracking Task")
            help:       qsTr("Track specified class using a paritcular model for inteferencing")
            modeValue:  QGroundControl.TrackingTask
        }
    }

    Column {
        spacing: ScreenTools.defaultFontPixelWidth

        Repeater {
            id: buttonRepeater

            Button {
                hoverEnabled:   true
                checked:        modeValue == currentSEMode

                background: Rectangle {
                    radius: ScreenTools.defaultFontPixelHeight / 2
                    color:  pressed | hovered | checked ? QGroundControl.globalPalette.buttonHighlight: QGroundControl.globalPalette.button
                }

                contentItem: Column {
                    spacing: 0

                    QGCLabel {
                        id:     modeNameLabel
                        text:   modeName
                        color:  pressed | hovered | checked ? QGroundControl.globalPalette.buttonHighlightText: QGroundControl.globalPalette.buttonText
                    }

                    QGCLabel {
                        width:              ScreenTools.defaultFontPixelWidth * 40
                        text:               help
                        wrapMode:           Label.WordWrap
                        font.pointSize:     ScreenTools.smallFontPointSize
                        color:              modeNameLabel.color
                    }
                }

                onClicked: {
                    updateSEModeFn(modeValue)
                    close()
                }
            }
        }
    }
}
