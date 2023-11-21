Label{
    id:axis6text
    anchors.right:axis6.left
    anchors.rightMargin: 82
    font.pointSize:(mainrect.height*0.06 + mainrect.width*0.10)/13
    anchors.bottom: axis6.top
    anchors.bottomMargin: -5
    color:"white"
    text:"Tip"
}
Rectangle {
    id: axis6
    height: (mainrect.height+mainrect.width)/550
    width: (mainrect.height+mainrect.width)/6
    border.color: "dark gray"
    color:"dark grey"
    border.width: 5
    anchors.left: controllerrect.left
    anchors.leftMargin: 100
    anchors.top: axis5.bottom
    anchors.topMargin: 50
    Label{
        id:axis6u
        text:"R"
        font.pointSize:(mainrect.height*0.06 + mainrect.width*0.10)/17
        anchors.right: axis6.right
        anchors.bottom: axis6.top
        color:"white"
    }
    Label{
        id:axis6d
        text:"L"
        font.pointSize:(mainrect.height*0.06 + mainrect.width*0.10)/17
        anchors.left: axis6.left
        anchors.bottom: axis6.top
        color:"white"
    }
    Image {
        id: axis6mover
        source: "file:///C:/Users/chand/Desktop/Qt_controls_Unity/Qt_controls_Unity/images/axisarrow.png"
        width: (mainrect.height*0.10 +  mainrect.width*0.10)/8.7
        height:  (mainrect.height*0.10 +  mainrect.width*0.10)/8.7
        anchors.top: axis6.bottom
        x: axis6.width / 2 - axis6mover.width / 2
        MouseArea {
            id: axis6mouse
            anchors.fill: parent
            drag.target: axis6mover
            drag.axis: Drag.XAxis
            onPositionChanged: {
                axis6mover.x < (axis6.width - axis6mover.width) / 2 ? axis6d.font.pointSize=15 : axis6u.font.pointSize=15
                axis6mover.x < (axis6.width - axis6mover.width) / 2 ? axis6u.font.pointSize=10 : axis6d.font.pointSize=10
                if(startstop && speedometer.value > 0){
                    axis6mover.x < (axis6.width - axis6mover.width) / 2 ? unityWidgetClass.changeVal("tipleft|"+speedometer.value) : unityWidgetClass.changeVal("tipright|"+speedometer.value);
                    unityWidgetClass.sendval("t")
                }
                else{
                    change()
                }
            }
            onPressed: {
                axis6text.font.bold=true
                axis6text.font.pointSize =15
                axis6.height = (mainrect.height+mainrect.width)/400
                axis6u.font.bold=true
                axis6d.font.bold=true
            }
            onReleased: {
                unityWidgetClass.stopSending()
                unityWidgetClass.sendval("n")
                axis6text.font.bold=false
                axis6u.font.pointSize=10
                axis6d.font.pointSize=10
                axis6text.font.pointSize =(mainrect.height*0.06 + mainrect.width*0.10)/13
                axis6.height = (mainrect.height+mainrect.width)/550
                axis6d.font.bold=false
                axis6u.font.bold=false
                axis6mover.x = axis6.width / 2 - axis6mover.width / 2;
            }
        }
        onXChanged: {
            if (axis6mover.x < 0) {
                axis6mover.x = 0;
            } else if (axis6mover.x > axis6.width - axis6mover.width) {
                axis6mover.x = axis6.width - axis6mover.width;
            }
        }
    }
}
