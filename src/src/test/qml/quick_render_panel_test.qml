import QtQuick 2.0
import QtQuick.Controls 2.2
import ros.rviz 1.0
import MyModule 1.0

ApplicationWindow {
  id: root
  width: 1024
  height: 768
  visible: true


  Loader {
    id: loader
    anchors.fill: parent
    sourceComponent: rvizComp
    // active: false
    asynchronous: true
    // visible: status == Loader.Ready
    Text{
      z:10
      id: progressView
      text: parent.progress
    }
  }
  Component {
    id:rvizComp
    Item {
      id:compView
      VisualizationFrame {
        id: visualizationFrame
        anchors.fill: parent
        renderWindow: renderWindow
      }

      Rectangle {
        anchors.fill: parent
        color: "lightblue"

        RenderWindow {
          id: renderWindow
          anchors.fill: parent
          // anchors.margins: 20
        }
      }

      SimpleGrid {
        id: grid
        frame: visualizationFrame
        lineWidth: 10
        color: "lightblue"
      }

      DisplayConfig {
        id: displayConfig
        frame: visualizationFrame
        //source: rvizPath + "/src/test/quick_test.rviz"
        source:"/home/tomo/rviz_mod/src/default.rviz"
        //source:"/home/quang/Downloads/rviz_qml/src/src/test/quick_test.rviz"
        //source:"/home/quang/Downloads/rviz_qml/src/src/test/rviz_logo.rviz"
        //source:"/home/quang/Downloads/rviz_qml/src/moveitAA.rviz"
      }
    }
  }
  // Timer{
  //   id: refreshRviz
  //   interval:2000
  //   onTriggered:{
  //     loader.active = true;
  //   }
  // }
  Row {
    anchors.top: parent.top
    anchors.left: parent.left
    Button {
      text: qsTr("reload")
      onClicked: {
        loader.active = false;
        loader.active = true;
      }
    }
    Button {
      text: "Red Grid"
      onClicked: grid.color = "red"
    }

    Button {
      text: "Blue Grid"
      onClicked: grid.color = "blue"
    }
    Button {
      text: "source 1"
      onClicked: {
        displayConfig.source="/home/quang/Downloads/rviz_qml/src/moveit.rviz"
      }
    }
    Button {
      text: "visible"
      onClicked: {
        loader.visible=!loader.visible
      }
    }
  }
}

