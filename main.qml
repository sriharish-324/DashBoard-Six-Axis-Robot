import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick 2.12

//https://forum.qt.io/topic/91928/how-to-create-exe-file-of-qt-project/5
//windeployqt.exe --qmldir C:\Qt\Qt_controls_Unity C:\Qt\Qt_controls_Unity\buildDirectory\Qt_controls_Unity.exe --dir C:\Users\sriha\Desktop\unit --release
Rectangle{
    id:mainrect
    color:"black"
    height:1020
    width:940
    anchors.right: parent.right
    anchors.left: parent.left
    anchors.top:parent.top
    TabView {
        id: frame
        anchors.fill: mainrect
        anchors.margins: 4
        Tab { title: "ControlPanel"
            ControlPannel{
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top:parent.top
                anchors.bottom:parent.bottom
            }
        }
        Tab { title: "TeachPanel"
            TeachPanel{
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top:parent.top
                anchors.bottom:parent.bottom
            }
        }
        style: TabViewStyle {
            frameOverlap: 1
            tab: Rectangle {
                color: styleData.selected ? "black" :"lightsteelblue"
                implicitWidth: mainrect.width*0.50
                implicitHeight: mainrect.height*0.05
                radius: 2
                Text {
                    id: tabtext
                    anchors.centerIn: parent
                    text: styleData.title
                    font.bold:true
                    color: styleData.selected ? "#00FFFF" : "white"
                    font.pointSize: 15
                }
            }
            frame: Rectangle { color: "black"
            }
        }
    }
}
