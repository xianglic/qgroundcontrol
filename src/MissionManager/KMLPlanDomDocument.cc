/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "KMLPlanDomDocument.h"
#include "QGCPalette.h"
#include "QGCApplication.h"
#include "MissionCommandTree.h"
#include "MissionCommandUIInfo.h"
#include "FactMetaData.h"
#include "ComplexMissionItem.h"
#include "QmlObjectListModel.h"

#include <QDomDocument>
#include <QStringList>

#include "TransectStyleComplexItem.h"
#include "StructureScanComplexItem.h"
#include "SimpleMissionItem.h"

#include <iostream>
#include <QVariant>
#include <QString>
#include <iostream>
#include <iomanip>

const char* KMLPlanDomDocument::_missionLineStyleName =     "MissionLineStyle";
const char* KMLPlanDomDocument::surveyPolygonStyleName =   "SurveyPolygonStyle";

KMLPlanDomDocument::KMLPlanDomDocument()
    : KMLDomDocument(QStringLiteral("%1 Plan KML").arg(qgcApp()->applicationName()))
{
    _addStyles();
}

// void KMLPlanDomDocument::_formatKML(){

// }
void KMLPlanDomDocument::_addCustomizedTask(Vehicle* vehicle, QList<MissionItem*> rgMissionItems, QmlObjectListModel* visualItems){

    // build up the flight path points
    int taskCounter = 1;
    for (int i=0; i<visualItems->count(); i++) {
        // debug
        std::cout << "chen test " << "\n";
        TransectStyleComplexItem* complexItem = visualItems->value<TransectStyleComplexItem*>(i);
        StructureScanComplexItem* structureScanItem = visualItems->value<StructureScanComplexItem*>(i);
        if (complexItem) {

            // add place mark
            QDomElement placemarkElement = createElement("Placemark");
            _rootDocumentElement.appendChild(placemarkElement);
            addTextElement(placemarkElement, "name",         "SteelEagleTask"+ QString::number(taskCounter));
            taskCounter++;

            // add description of the task
            QDomElement descriptionElement = createElement("description");
            QString htmlString;
            QString convertedValue;
            Fact* detectFact = (complexItem->detectTask());
            // Read the value using the appropriate getter function
            QVariant value = detectFact->cookedValue();
            // Convert the QVariant to the desired type if needed
            convertedValue = value.toString();
            htmlString += QStringLiteral("DetectTask: {model: '%1'}\n").arg(convertedValue);
            QDomCDATASection cdataSection = createCDATASection(htmlString);
            descriptionElement.appendChild(cdataSection);
            placemarkElement.appendChild(descriptionElement);


            // Build up all missions points for task
            QList<QGeoCoordinate> rgFlightCoords;

            QmlObjectListModel* subseqItemsList = complexItem->flightPathSegments();
            for (int i=0; i<subseqItemsList->count(); i++){
                FlightPathSegment* flightPath = subseqItemsList->value<FlightPathSegment*>(i);
                QGeoCoordinate coordi_1 = flightPath->coordinate1();
                double altitude = qIsNaN(flightPath->coord1AMSLAlt() ) ? 0 : flightPath->coord1AMSLAlt();
                coordi_1.setAltitude(altitude);

                //debug
                std::cout << std::fixed << std::setprecision(7) << "subLongitude1: " << coordi_1.longitude() << " ";
                std::cout << std::fixed << std::setprecision(7) << "subLatitude1: " << coordi_1.latitude() << " ";
                std::cout << std::fixed << std::setprecision(7) << "subaltitude1: " << altitude << std::endl;

                rgFlightCoords += coordi_1;
                
                if (i == subseqItemsList->count() - 1){ // last one coordinate 2 must be included
                    QGeoCoordinate coordi_2 = flightPath->coordinate2();
                    double altitude = qIsNaN(flightPath->coord2AMSLAlt() ) ? 0 : flightPath->coord2AMSLAlt();
                    coordi_2.setAltitude(altitude);

                    // debug
                    std::cout << std::fixed << std::setprecision(7) << "subLongitude2: " << coordi_2.longitude() << " ";
                    std::cout << std::fixed << std::setprecision(7) << "subLatitude2: " << coordi_2.latitude() << " ";
                    std::cout << std::fixed << std::setprecision(7) << "subaltitude2: " << altitude << std::endl;
                   
                    
                    rgFlightCoords += coordi_2;
                }
            }

            // Create a LineString element from the coords
            QDomElement lineStringElement = createElement("LineString");
            placemarkElement.appendChild(lineStringElement);

            QString coordString;
            for (const QGeoCoordinate& coord : rgFlightCoords) {
                coordString += QStringLiteral("%1\n").arg(kmlCoordString(coord));
            }
            addTextElement(lineStringElement, "coordinates", coordString);
        }else if (structureScanItem){
            std::cout << "HI ";
            Fact* detectModel = (structureScanItem->detectModel());
            // Read the value using the appropriate getter function
            QVariant value = detectModel->cookedValue();
            // Convert the QVariant to the desired type if needed
            QString convertedValue = value.toString();
            std::cout << "HI "<<convertedValue.toStdString();
            
            // add place mark
            QDomElement placemarkElement = createElement("Placemark");
            _rootDocumentElement.appendChild(placemarkElement);
            addTextElement(placemarkElement, "name",         "SteelEagleTask"+ QString::number(taskCounter));
            taskCounter++;

            // add description of the task
            QDomElement descriptionElement = createElement("description");
            QString htmlString;
            htmlString += QStringLiteral("DetectTask: {model: '%1'}\n").arg(convertedValue);
            QDomCDATASection cdataSection = createCDATASection(htmlString);
            descriptionElement.appendChild(cdataSection);
            placemarkElement.appendChild(descriptionElement);


            // Build up all missions points for task
            QList<QGeoCoordinate> rgFlightCoords;

            QmlObjectListModel* subseqItemsList = structureScanItem->flightPathSegments();
            for (int i=0; i<subseqItemsList->count(); i++){
                FlightPathSegment* flightPath = subseqItemsList->value<FlightPathSegment*>(i);
                QGeoCoordinate coordi_1 = flightPath->coordinate1();
                double altitude = qIsNaN(flightPath->coord1AMSLAlt() ) ? 0 : flightPath->coord1AMSLAlt();
                coordi_1.setAltitude(altitude);

                //debug
                std::cout << std::fixed << std::setprecision(7) << "subLongitude1: " << coordi_1.longitude() << " ";
                std::cout << std::fixed << std::setprecision(7) << "subLatitude1: " << coordi_1.latitude() << " ";
                std::cout << std::fixed << std::setprecision(7) << "subaltitude1: " << altitude << std::endl;

                rgFlightCoords += coordi_1;
                
                if (i == subseqItemsList->count() - 1){ // last one coordinate 2 must be included
                    QGeoCoordinate coordi_2 = flightPath->coordinate2();
                    double altitude = qIsNaN(flightPath->coord2AMSLAlt() ) ? 0 : flightPath->coord2AMSLAlt();
                    coordi_2.setAltitude(altitude);

                    // debug
                    std::cout << std::fixed << std::setprecision(7) << "subLongitude2: " << coordi_2.longitude() << " ";
                    std::cout << std::fixed << std::setprecision(7) << "subLatitude2: " << coordi_2.latitude() << " ";
                    std::cout << std::fixed << std::setprecision(7) << "subaltitude2: " << altitude << std::endl;
                   
                    
                    rgFlightCoords += coordi_2;
                }
            }

            // Create a LineString element from the coords
            QDomElement lineStringElement = createElement("LineString");
            placemarkElement.appendChild(lineStringElement);

            QString coordString;
            for (const QGeoCoordinate& coord : rgFlightCoords) {
                coordString += QStringLiteral("%1\n").arg(kmlCoordString(coord));
            }
            addTextElement(lineStringElement, "coordinates", coordString);
        }
    }
}


void KMLPlanDomDocument::_addFlightPath(Vehicle* vehicle, QList<MissionItem*> rgMissionItems)
{
    if (rgMissionItems.count() == 0) {
        return;
    }

    QDomElement itemFolderElement = createElement("Folder");
    _rootDocumentElement.appendChild(itemFolderElement);

    addTextElement(itemFolderElement, "name", "Items");

    QDomElement flightPathElement = createElement("Placemark");
    _rootDocumentElement.appendChild(flightPathElement);

    addTextElement(flightPathElement, "styleUrl",     QStringLiteral("#%1").arg(_missionLineStyleName));
    addTextElement(flightPathElement, "name",         "Flight Path");
    addTextElement(flightPathElement, "visibility",   "1");
    addLookAt(flightPathElement, rgMissionItems[0]->coordinate());

    // Build up the mission trajectory line coords
    QList<QGeoCoordinate> rgFlightCoords;
    QGeoCoordinate homeCoord = rgMissionItems[0]->coordinate();
    for (const MissionItem* item : rgMissionItems) {
        const MissionCommandUIInfo* uiInfo = qgcApp()->toolbox()->missionCommandTree()->getUIInfo(vehicle, QGCMAVLink::VehicleClassGeneric, item->command());
        if (uiInfo) {
            double altAdjustment = item->frame() == MAV_FRAME_GLOBAL ? 0 : homeCoord.altitude(); // Used to convert to amsl
            if (uiInfo->isTakeoffCommand() && !vehicle->fixedWing()) {
                // These takeoff items go straight up from home position to specified altitude
                QGeoCoordinate coord = homeCoord;
                coord.setAltitude(item->param7() + altAdjustment);
                rgFlightCoords += coord;
            }
            if (uiInfo->specifiesCoordinate()) {
                QGeoCoordinate coord = item->coordinate();
                coord.setAltitude(coord.altitude() + altAdjustment); // convert to amsl

                if (!uiInfo->isStandaloneCoordinate()) {
                    // Flight path goes through this item
                    rgFlightCoords += coord;
                }

                // Add a place mark for each WP

                QDomElement wpPlacemarkElement = createElement("Placemark");
                addTextElement(wpPlacemarkElement, "name",     QStringLiteral("%1 %2").arg(QString::number(item->sequenceNumber())).arg(item->command() == MAV_CMD_NAV_WAYPOINT ? "" : uiInfo->friendlyName()));
                addTextElement(wpPlacemarkElement, "styleUrl", QStringLiteral("#%1").arg(balloonStyleName));

                QDomElement wpPointElement = createElement("Point");
                addTextElement(wpPointElement, "altitudeMode", "absolute");
                addTextElement(wpPointElement, "coordinates",  kmlCoordString(coord));
                addTextElement(wpPointElement, "extrude",      "1");

                QDomElement descriptionElement = createElement("description");
                QString htmlString;
                htmlString += QStringLiteral("Index: %1\n").arg(item->sequenceNumber());
                htmlString += uiInfo->friendlyName() + "\n";
                htmlString += QStringLiteral("Alt AMSL: %1 %2\n").arg(QString::number(FactMetaData::metersToAppSettingsHorizontalDistanceUnits(coord.altitude()).toDouble(), 'f', 2)).arg(FactMetaData::appSettingsHorizontalDistanceUnitsString());
                htmlString += QStringLiteral("Alt Rel: %1 %2\n").arg(QString::number(FactMetaData::metersToAppSettingsHorizontalDistanceUnits(coord.altitude() - homeCoord.altitude()).toDouble(), 'f', 2)).arg(FactMetaData::appSettingsHorizontalDistanceUnitsString());
                htmlString += QStringLiteral("Lat: %1\n").arg(QString::number(coord.latitude(), 'f', 7));
                htmlString += QStringLiteral("Lon: %1\n").arg(QString::number(coord.longitude(), 'f', 7));
                QDomCDATASection cdataSection = createCDATASection(htmlString);
                descriptionElement.appendChild(cdataSection);

                wpPlacemarkElement.appendChild(descriptionElement);
                wpPlacemarkElement.appendChild(wpPointElement);
                itemFolderElement.appendChild(wpPlacemarkElement);
            }
        }
    }

    // Create a LineString element from the coords

    QDomElement lineStringElement = createElement("LineString");
    flightPathElement.appendChild(lineStringElement);

    addTextElement(lineStringElement, "extruder",      "1");
    addTextElement(lineStringElement, "tessellate",    "1");
    addTextElement(lineStringElement, "altitudeMode",  "absolute");

    QString coordString;
    for (const QGeoCoordinate& coord : rgFlightCoords) {
        coordString += QStringLiteral("%1\n").arg(kmlCoordString(coord));
    }
    addTextElement(lineStringElement, "coordinates", coordString);
}

void KMLPlanDomDocument::_addComplexItems(QmlObjectListModel* visualItems)
{
    for (int i=0; i<visualItems->count(); i++) {
        ComplexMissionItem* complexItem = visualItems->value<ComplexMissionItem*>(i);
        if (complexItem) {
            complexItem->addKMLVisuals(*this);
        }
    }
}

void KMLPlanDomDocument::addMission(Vehicle* vehicle, QmlObjectListModel* visualItems, QList<MissionItem*> rgMissionItems)
{
    // _addFlightPath(vehicle, rgMissionItems);
    // _addComplexItems(visualItems);
    _addCustomizedTask(vehicle, rgMissionItems,  visualItems);

}

void KMLPlanDomDocument::_addStyles(void)
{
    QGCPalette palette;

    QDomElement styleElement1 = createElement("Style");
    styleElement1.setAttribute("id", _missionLineStyleName);
    QDomElement lineStyleElement = createElement("LineStyle");
    addTextElement(lineStyleElement, "color", kmlColorString(palette.mapMissionTrajectory()));
    addTextElement(lineStyleElement, "width", "4");
    styleElement1.appendChild(lineStyleElement);

    QString kmlSurveyColorString = kmlColorString(palette.surveyPolygonInterior(), 0.5 /* opacity */);
    QDomElement styleElement2 = createElement("Style");
    styleElement2.setAttribute("id", surveyPolygonStyleName);
    QDomElement polygonStyleElement = createElement("PolyStyle");
    addTextElement(polygonStyleElement, "color", kmlSurveyColorString);
    QDomElement polygonLineStyleElement = createElement("LineStyle");
    addTextElement(polygonLineStyleElement, "color", kmlSurveyColorString);
    styleElement2.appendChild(polygonStyleElement);
    styleElement2.appendChild(polygonLineStyleElement);

    _rootDocumentElement.appendChild(styleElement1);
    _rootDocumentElement.appendChild(styleElement2);
}
