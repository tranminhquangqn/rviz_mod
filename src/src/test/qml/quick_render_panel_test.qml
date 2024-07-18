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
    asynchronous: true
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
        // source:"/home/quang/rviz_mod/src/default.rviz" //Set this directly in test/displaycongig.cpp
      }
      Row {
        anchors.top: parent.top
        anchors.left: parent.left
        Button {
          text: "Red Grid"
          onClicked: grid.color = "red"
        }
        Button {
          text: "Blue Grid"
          onClicked: grid.color = "blue"
        }
        Button {
          text: "Default Grid"
          onClicked: grid.color = "lightblue"
        }
      }
    }
  }
  Button {
    text: "visible"
    anchors.horizontalCenter: parent.horizontalCenter
    onClicked: {
      loader.visible=!loader.visible
    }
  }
}

