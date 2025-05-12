import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    visible: true
    width: 400
    height: 400
    title: "Chess"

    Grid {
        id: grid
        columns: 8
        rows: 8
        anchors.fill: parent
        anchors.margins: 10
        Repeater {
            model: board
            delegate: Rectangle {
                width: grid.width / 8
                height: grid.height / 8
                color: (row + col) % 2 === 0 ? "white" : "darkgoldenrod"
                Text {
                    anchors.centerIn: parent
                    text: piece
                    font.pixelSize: 48
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        controller.selectOrMovePiece(row, col)
                    }
                }
            }
        }
    }
}
