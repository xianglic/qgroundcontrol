/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "ComplexMissionItem.h"
#include "MissionItem.h"
#include "SettingsFact.h"
#include "QGCLoggingCategory.h"
#include "QGCMapPolyline.h"
#include "QGCMapPolygon.h"
#include "CameraCalc.h"
#include "TerrainQuery.h"

Q_DECLARE_LOGGING_CATEGORY(TransectStyleComplexItemLog)

class PlanMasterController;

class TransectStyleComplexItem : public ComplexMissionItem
{
    Q_OBJECT

public:
    TransectStyleComplexItem(PlanMasterController* masterController, bool flyView, QString settignsGroup);

    // // detect task
    // Q_PROPERTY(Fact*            detectTask                  READ detectTask                                         CONSTANT)
    // Q_PROPERTY(Fact*            detectTask_gimbal_pitch     READ detectTask_gimbal_pitch                            CONSTANT)
    // Q_PROPERTY(Fact*            detectTask_model            READ detectTask_model                                   CONSTANT)
    // Q_PROPERTY(Fact*            detectTask_drone_rotation   READ detectTask_drone_rotation                          CONSTANT)
    // Q_PROPERTY(Fact*            detectTask_sample_rate      READ detectTask_sample_rate                             CONSTANT) 
    // Q_PROPERTY(Fact*            detectTask_hover_delay      READ detectTask_hover_delay                             CONSTANT)
    // Q_PROPERTY(QStringList      detectTask_modellist        MEMBER _detectTask_modellist                            CONSTANT)
    // Q_PROPERTY(QString          detectTask_model_2          MEMBER _detectTask_model_2                              WRITE setDetectTask_model)
    
    // // obstacle task
    // Q_PROPERTY(Fact*            obstacleTask_model          READ obstacleTask_model                                 CONSTANT)
    // Q_PROPERTY(Fact*            obstacleTask_speed          READ obstacleTask_speed                                 CONSTANT)
    // Q_PROPERTY(Fact*            obstacleTask_altitude       READ obstacleTask_altitude                              CONSTANT)

    // // tracking task 
    // Q_PROPERTY(Fact*            trackingTask_gimbal_pitch   READ trackingTask_gimbal_pitch                          CONSTANT)
    // Q_PROPERTY(Fact*            trackingTask_model          READ trackingTask_model                                 CONSTANT)
    // Q_PROPERTY(Fact*            trackingTask_class          READ trackingTask_class                                 CONSTANT)
    // Q_PROPERTY(QStringList      trackingTask_modellist        MEMBER _trackingTask_modellist                        CONSTANT)
    // Q_PROPERTY(QString          trackingTask_model_2          MEMBER _trackingTask_model_2                          WRITE setTrackingTask_model)
    
    Q_PROPERTY(QGCMapPolygon*   surveyAreaPolygon           READ surveyAreaPolygon                                  CONSTANT)
    Q_PROPERTY(CameraCalc*      cameraCalc                  READ cameraCalc                                         CONSTANT)
    Q_PROPERTY(Fact*            turnAroundDistance          READ turnAroundDistance                                 CONSTANT)
    Q_PROPERTY(Fact*            cameraTriggerInTurnAround   READ cameraTriggerInTurnAround                          CONSTANT)
    Q_PROPERTY(Fact*            hoverAndCapture             READ hoverAndCapture                                    CONSTANT)
    Q_PROPERTY(Fact*            refly90Degrees              READ refly90Degrees                                     CONSTANT)

    Q_PROPERTY(int              cameraShots                 READ cameraShots                                        NOTIFY cameraShotsChanged)
    Q_PROPERTY(double           timeBetweenShots            READ timeBetweenShots                                   NOTIFY timeBetweenShotsChanged)
    Q_PROPERTY(double           coveredArea                 READ coveredArea                                        NOTIFY coveredAreaChanged)
    Q_PROPERTY(bool             hoverAndCaptureAllowed      READ hoverAndCaptureAllowed                             CONSTANT)
    Q_PROPERTY(QVariantList     visualTransectPoints        READ visualTransectPoints                               NOTIFY visualTransectPointsChanged)
    Q_PROPERTY(Fact*            terrainAdjustTolerance      READ terrainAdjustTolerance                             CONSTANT)
    Q_PROPERTY(Fact*            terrainAdjustMaxDescentRate READ terrainAdjustMaxDescentRate                        CONSTANT)
    Q_PROPERTY(Fact*            terrainAdjustMaxClimbRate   READ terrainAdjustMaxClimbRate                          CONSTANT)

    QGCMapPolygon*  surveyAreaPolygon   (void) { return &_surveyAreaPolygon; }
    CameraCalc*     cameraCalc          (void) { return &_cameraCalc; }
    QVariantList    visualTransectPoints(void) { return _visualTransectPoints; }

    Fact* turnAroundDistance            (void) { return &_turnAroundDistanceFact; }

    // //detect task
    // Fact* detectTask                    (void) { return &_detectTaskFact; }
    // Fact* detectTask_gimbal_pitch       (void) { return &_detectTask_gimbal_pitchFact; }
    // Fact* detectTask_model              (void) {return &_detectTask_modelFact;}
    // Fact* detectTask_drone_rotation     (void) {return &_detectTask_drone_rotationFact;}
    // Fact* detectTask_sample_rate        (void) {return &_detectTask_sample_rateFact;} 
    // Fact* detectTask_hover_delay        (void) {return &_detectTask_hover_delayFact;}

    // void  setDetectTask_model           (const QString& detectTask_model);
    // QString  detectTask_model_2        (void) {return _detectTask_model_2;}
        
    // // obstacle task

    // Fact* obstacleTask_model            (void) { return &_obstacleTask_modelFact; }
    // Fact* obstacleTask_speed            (void) { return &_obstacleTask_speedFact; }
    // Fact* obstacleTask_altitude         (void) {return &_obstacleTask_altitudeFact;}


    // // tracking task
    // Fact* trackingTask_gimbal_pitch     (void) { return &_trackingTask_gimbal_pitchFact; }
    // Fact* trackingTask_model            (void) { return &_trackingTask_modelFact; }
    // Fact* trackingTask_class            (void) {return &_trackingTask_classFact;} 


    // void  setTrackingTask_model          (const QString& trackingTask_model);
    // QString  trackingTask_model_2        (void) {return _trackingTask_model_2;}


    Fact* cameraTriggerInTurnAround     (void) { return &_cameraTriggerInTurnAroundFact; }
    Fact* hoverAndCapture               (void) { return &_hoverAndCaptureFact; }
    Fact* refly90Degrees                (void) { return &_refly90DegreesFact; }
    Fact* terrainAdjustTolerance        (void) { return &_terrainAdjustToleranceFact; }
    Fact* terrainAdjustMaxDescentRate   (void) { return &_terrainAdjustMaxDescentRateFact; }
    Fact* terrainAdjustMaxClimbRate     (void) { return &_terrainAdjustMaxClimbRateFact; }

    const Fact* hoverAndCapture         (void) const { return &_hoverAndCaptureFact; }

    int             cameraShots             (void) const { return _cameraShots; }
    double          coveredArea             (void) const;
    bool            hoverAndCaptureAllowed  (void) const;

    virtual double  timeBetweenShots        (void) { return 0; } // Most be overridden. Implementation here is needed for unit testing.

    double  triggerDistance         (void) const { return _cameraCalc.adjustedFootprintFrontal()->rawValue().toDouble(); }
    bool    hoverAndCaptureEnabled  (void) const { return hoverAndCapture()->rawValue().toBool(); }
    bool    triggerCamera           (void) const { return triggerDistance() != 0; }

    // Used internally only by unit tests
    int _transectCount(void) const { return _transects.count(); }

    // Overrides from ComplexMissionItem
    int     lastSequenceNumber  (void) const final;
    QString mapVisualQML        (void) const override = 0;
    bool    load                (const QJsonObject& complexObject, int sequenceNumber, QString& errorString) override = 0;
    void    addKMLVisuals       (KMLPlanDomDocument& domDocument) final;
    double  complexDistance     (void) const final { return _complexDistance; }
    double  greatestDistanceTo  (const QGeoCoordinate &other) const final;

    // Overrides from VisualMissionItem
    void                save                        (QJsonArray&  planItems) override = 0;
    bool                specifiesCoordinate         (void) const override = 0;
    void                appendMissionItems          (QList<MissionItem*>& items, QObject* missionItemParent) final;
    void                applyNewAltitude            (double newAltitude) final;
    bool                dirty                       (void) const final { return _dirty; }
    bool                isSimpleItem                (void) const final { return false; }
    bool                isStandaloneCoordinate      (void) const final { return false; }
    bool                specifiesAltitudeOnly       (void) const final { return false; }
    QGeoCoordinate      coordinate                  (void) const final { return _coordinate; }
    QGeoCoordinate      exitCoordinate              (void) const final { return _exitCoordinate; }
    int                 sequenceNumber              (void) const final { return _sequenceNumber; }
    double              specifiedFlightSpeed        (void) final { return std::numeric_limits<double>::quiet_NaN(); }
    double              specifiedGimbalYaw          (void) final { return std::numeric_limits<double>::quiet_NaN(); }
    double              specifiedGimbalPitch        (void) final { return std::numeric_limits<double>::quiet_NaN(); }
    void                setMissionFlightStatus      (MissionController::MissionFlightStatus_t& missionFlightStatus) final;
    ReadyForSaveState   readyForSaveState         (void) const override;
    QString             commandDescription          (void) const override { return tr("Transect"); }
    QString             commandName                 (void) const override { return tr("Transect"); }
    QString             abbreviation                (void) const override { return tr("T"); }
    bool                exitCoordinateSameAsEntry   (void) const final { return false; }
    void                setDirty                    (bool dirty) final;
    void                setCoordinate               (const QGeoCoordinate& coordinate) final { Q_UNUSED(coordinate); }
    void                setSequenceNumber           (int sequenceNumber) final;
    double              amslEntryAlt                (void) const final;
    double              amslExitAlt                 (void) const final;
    double              minAMSLAltitude             (void) const final;
    double              maxAMSLAltitude             (void) const final;

    // //detect task
    // static const char* detectTaskName;
    // static const char* detectTask_gimbal_pitchName;
    // static const char* detectTask_modelName;            
    // static const char* detectTask_drone_rotationName;
    // static const char* detectTask_sample_rateName;
    // static const char* detectTask_hover_delayName;

    // // obstacle task
    // static const char* obstacleTask_modelName;
    // static const char* obstacleTask_speedName;
    // static const char* obstacleTask_altitudeName;  


    // // tracking task
    // static const char* trackingTask_gimbal_pitchName;
    // static const char* trackingTask_modelName;
    // static const char* trackingTask_className;  

    

    static const char* turnAroundDistanceName;
    static const char* turnAroundDistanceMultiRotorName;
    static const char* cameraTriggerInTurnAroundName;
    static const char* hoverAndCaptureName;
    static const char* refly90DegreesName;
    static const char* terrainAdjustToleranceName;
    static const char* terrainAdjustMaxClimbRateName;
    static const char* terrainAdjustMaxDescentRateName;

signals:
    void cameraShotsChanged             (void);
    void timeBetweenShotsChanged        (void);
    void visualTransectPointsChanged    (void);
    void coveredAreaChanged             (void);
    void _updateFlightPathSegmentsSignal(void);

protected slots:
    void _setDirty                          (void);
    void _setIfDirty                        (bool dirty);
    void _updateCoordinateAltitudes         (void);
    void _polyPathTerrainData               (bool success, const QList<TerrainPathQuery::PathHeightInfo_t>& rgPathHeightInfo);
    void _missionItemCoordTerrainData       (bool success, QList<double> heights);
    void _rebuildTransects                  (void);

protected:
    virtual void _rebuildTransectsPhase1    (void) = 0; ///< Rebuilds the _transects array
    virtual void _recalcCameraShots         (void) = 0;

    void    _save                           (QJsonObject& saveObject);
    bool    _load                           (const QJsonObject& complexObject, bool forPresets, QString& errorString);
    void    _setExitCoordinate              (const QGeoCoordinate& coordinate);
    void    _setCameraShots                 (int cameraShots);
    double  _triggerDistance                (void) const;
    bool    _hasTurnaround                  (void) const;
    double  _turnAroundDistance             (void) const;
    // QString _detectTask                     (void) const;
    void    _appendWaypoint                 (QList<MissionItem*>& items, QObject* missionItemParent, int& seqNum, MAV_FRAME mavFrame, float holdTime, const QGeoCoordinate& coordinate);
    void    _appendSinglePhotoCapture       (QList<MissionItem*>& items, QObject* missionItemParent, int& seqNum);
    void    _appendConditionGate            (QList<MissionItem*>& items, QObject* missionItemParent, int& seqNum, MAV_FRAME mavFrame, const QGeoCoordinate& coordinate);
    void    _appendCameraTriggerDistance    (QList<MissionItem*>& items, QObject* missionItemParent, int& seqNum, float triggerDistance);
    void    _appendCameraTriggerDistanceUpdatePoint(QList<MissionItem*>& items, QObject* missionItemParent, int& seqNum, MAV_FRAME mavFrame, const QGeoCoordinate& coordinate, bool useConditionGate, float triggerDistance);
    void    _buildAndAppendMissionItems     (QList<MissionItem*>& items, QObject* missionItemParent);
    void    _appendLoadedMissionItems       (QList<MissionItem*>& items, QObject* missionItemParent);
    void    _recalcComplexDistance          (void);

    int                 _sequenceNumber = 0;
    QGeoCoordinate      _coordinate;
    QGeoCoordinate      _exitCoordinate;
    QGCMapPolygon       _surveyAreaPolygon;

    enum CoordType {
        CoordTypeInterior,              ///< Interior waypoint for flight path only (example: interior corridor point)
        CoordTypeInteriorHoverTrigger,  ///< Interior waypoint for hover and capture trigger
        CoordTypeInteriorTerrainAdded,  ///< Interior waypoint added for terrain
        CoordTypeSurveyEntry,           ///< Waypoint at entry edge of survey polygon
        CoordTypeSurveyExit,            ///< Waypoint at exit edge of survey polygon
        CoordTypeTurnaround,            ///< Turnaround extension waypoint
    };

    typedef struct {
        QGeoCoordinate  coord;
        CoordType       coordType;
    } CoordInfo_t;

    QVariantList                                _visualTransectPoints;                          ///< Used to draw the flight path visuals on the screen
    QList<QList<CoordInfo_t>>                   _transects;
    QList<TerrainPathQuery::PathHeightInfo_t>   _rgPathHeightInfo;                              ///< Path height for each segment includes turn segments
    QList<QGeoCoordinate>                       _rgFlyThroughMissionItemCoords;
    QList<double>                               _rgFlyThroughMissionItemCoordsTerrainHeights;
    QList<CoordInfo_t>                          _rgFlightPathCoordInfo;                         ///< Fully calculated flight path (including terrain if needed)

    bool            _ignoreRecalc =     false;
    double          _complexDistance =  qQNaN();
    int             _cameraShots =      0;
    double          _timeBetweenShots = 0;
    double          _vehicleSpeed =     5;
    CameraCalc      _cameraCalc;
    double          _minAMSLAltitude =  qQNaN();
    double          _maxAMSLAltitude =  qQNaN();

    QObject*            _loadedMissionItemsParent = nullptr;	///< Parent for all items in _loadedMissionItems for simpler delete
    QList<MissionItem*> _loadedMissionItems;                    ///< Mission items loaded from plan file

    QMap<QString, FactMetaData*> _metaDataMap;


    // // detect task
    // SettingsFact _detectTaskFact;
    // SettingsFact _detectTask_gimbal_pitchFact;
    // SettingsFact _detectTask_modelFact;            
    // SettingsFact _detectTask_drone_rotationFact;
    // SettingsFact _detectTask_sample_rateFact;
    // SettingsFact _detectTask_hover_delayFact;
    // QStringList  _detectTask_modellist;
    // QString      _detectTask_model_2 = "coco";
    
    // // obstacle task
    // SettingsFact _obstacleTask_modelFact;
    // SettingsFact _obstacleTask_speedFact;
    // SettingsFact _obstacleTask_altitudeFact; 

    // // tracking task
    // SettingsFact _trackingTask_gimbal_pitchFact;
    // SettingsFact _trackingTask_modelFact;
    // SettingsFact _trackingTask_classFact;
    // QStringList  _trackingTask_modellist;
    // QString      _trackingTask_model_2 = "coco";
      

    SettingsFact _turnAroundDistanceFact;
    SettingsFact _cameraTriggerInTurnAroundFact;
    SettingsFact _hoverAndCaptureFact;
    SettingsFact _refly90DegreesFact;
    SettingsFact _terrainAdjustToleranceFact;
    SettingsFact _terrainAdjustMaxClimbRateFact;
    SettingsFact _terrainAdjustMaxDescentRateFact;

    static const char* _jsonCameraCalcKey;
    static const char* _jsonTransectStyleComplexItemKey;
    static const char* _jsonVisualTransectPointsKey;
    static const char* _jsonItemsKey;
    static const char* _jsonTerrainFlightSpeed;
    static const char* _jsonCameraShotsKey;

    static const int _terrainQueryTimeoutMsecs=     1000;
    static const int _hoverAndCaptureDelaySeconds = 4;

private slots:
    void _reallyQueryTransectsPathHeightInfo        (void);
    void _handleHoverAndCaptureEnabled              (QVariant enabled);
    void _updateFlightPathSegmentsDontCallDirectly  (void);
    void _segmentTerrainCollisionChanged            (bool terrainCollision) final;
    void _distanceModeChanged                       (int distanceMode);

private:
    typedef struct {
        bool imagesInTurnaround;
        bool hasTurnarounds;
        bool addTriggerAtFirstAndLastPoint;
        bool useConditionGate;
        // bool hasDetectTask;
    } BuildMissionItemsState_t;

    void    _queryTransectsPathHeightInfo                                   (void);
    void    _queryMissionItemCoordHeights                                   (void);
    void    _adjustForAvailableTerrainData                                  (void);
    void    _buildFlightPathCoordInfoFromTransects                          (void);
    void    _buildFlightPathCoordInfoFromPathHeightInfoForCalcAboveTerrain  (void);
    void    _buildFlightPathCoordInfoFromPathHeightInfoForTerrainFrame      (void);
    void    _buildFlightPathCoordInfoFromMissionItems                       (void);
    void    _adjustForMaxRates                                              (void);
    void    _adjustForTolerance                                             (void);
    double  _altitudeBetweenCoords                                          (const QGeoCoordinate& fromCoord, const QGeoCoordinate& toCoord, double percentTowardsTo);
    int     _maxPathHeight                                                  (const TerrainPathQuery::PathHeightInfo_t& pathHeightInfo, int fromIndex, int toIndex, double& maxHeight);
    BuildMissionItemsState_t _buildMissionItemsState                        (void) const;

    TerrainPolyPathQuery*       _currentTerrainPolyPathQuery        = nullptr;
    TerrainAtCoordinateQuery*   _currentTerrainAtCoordinateQuery    = nullptr;
    QTimer                      _terrainPolyPathQueryTimer;

    // Deprecated json keys
    static const char* _jsonTerrainFollowKeyDeprecated;
};
