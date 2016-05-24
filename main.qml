import QtQuick 2.5
import QtQuick.Controls 1.4

// import Test 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    Label {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }

//    SampleTest{id : foo}

//    Component.onCompleted: {
//        var s = foo.createStruct();
//        console.log(s.setAge(100))
//        // s.setAge(10).setAge(20);
//        console.log(s.getAge());
//    }
}

