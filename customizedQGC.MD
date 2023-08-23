# Customized QGroundControl 

## Class Hierarchy

- A MissionItem is a class which represents the raw Mavlink MISSION_ITEM data. 
- A VisualMission is the base class for a visual display of a mission item. This requires a lot more information to display in a Plan View than which is associated with the raw mavlink message. 
- Then there are Simple and Complex visual items which layer above the base. Simple is just a waypoint, complex are the various patterns. You'll see that a simple item contains a MissionItem which holds the majority of the information. Complex items are more complicated. A Complex item contains the meta information associated with the pattern. 
- You can then convert that into a set of raw Mission Items which are eventually uploaded to the vehicle to run the mission. The vehicle side only understands raw mavlink mission items. It knowns nothing about simple/complex/visual items.

## Implementation Details

***Notice filename without suffix are basically .cc and .h***

### Corridor Scan mirror:

1. Useful Tip:  you can get all the modified files by searching keywords: detectTask and Corridor2

2. Mirror a corridor scan as a complex pattern (Corridor2):

- ​	In PlanView, we use create a qml file to design the UI in qgc for corridor scan mirror. 
- ​	In MissionManager, we create a corridor scan mirror class, its plan creator class, which basically defines the functions and features of corridor scan mirror. Notice Corridor2.SettingGroup.Json is for setting default values for these features. And We register these classes in Mission Controller and PlanMasterController, there two files are for bridging between the user "plan creating" command and actual plan generation in the back end.  
- ​	In root, we register all the files within different Makefile for building process.

3. Register a new feature in the mission item for a certain complex pattern (detectTask):

- ​	We basically register a new feature in the mirrored class "corridor2" by making a box in the right hand side of mission items in UI. We create detectTask as a "Fact" that is bond to the TransectStyleComplexItem.  And 

### KML Generation:

- Saving logic: the plan creating process is a click based event. Once PlanView receives a user click action for saving a complex plan, the action will trigger to call PlanMasterController, MissionController, and KMLPlanDomDocument to call function addMission(). The logic flow is drawn below as reference.
  - Save to KML: 
    - PlanView.qml -> click based event -> PlanMasterController -> MissionController-> KMLPlanDomDocument:: addMission
- Customized idea: Insert a new customized function in KMLPlanDomDocument to generate the KML in a format fit for Steal Eagle flight plan. It parse the new Fact "detectTask" out from the ComplexTransectStyleItem, takeoff point, destination point, and also the list of waypoints in the middle of the flight. And we can utilize the above information to construct the KML file fitting for our own flight task.

### Files related:

- ​	In MissionManager folder: 
- - CMakeList.txt
  - Corridor2
  - Corridor2PlanCreator
  - MissionController
  - PlanMasterController
  - TransectStyleComplexItem
  - Corridor2.SettingGroup.json
- ​	In PlanView:
- - ​		CMakeList.txt
  - ​		Corridor2Editor.qml
- ​	In root:
- - ​		qgcimages.qrc
  - ​		qgroundcontrol.pro
  - ​		qgroundcontrol.qrc

