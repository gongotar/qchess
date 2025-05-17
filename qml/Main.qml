import QtQuick

Window {
    title: "Chess"
    visible: true
    width: 400
    height: 400
    onWidthChanged: {
        height = width
    }
    onHeightChanged: {
        width = height
    }
    FontLoader {
        id: chessFont
        source: "qrc:/qt/qml/qchess/resources/Alpha.ttf"
    }
    GridView {
        id: grid
        anchors.fill: parent
        anchors.margins: 10
        cellWidth: width / 8
        cellHeight: height / 8
        model: board
        delegate: Rectangle {
            width: grid.width / 8
            height: grid.height / 8
            color: (row + col) % 2 === 0 ? "white" : "darkgoldenrod"
            Rectangle {
                visible: highlight
                anchors.fill: parent
                color: "gold"
                opacity: 0.22
            }
            Rectangle {
                visible: isSelected
                anchors.fill: parent
                color: "blue"
                opacity: 0.18
            }
            Rectangle {
                visible: isLegalDestination
                anchors.centerIn: parent
                width: parent.width * 0.4
                height: parent.height * 0.4
                radius: width / 2
                color: "black"
                opacity: 0.5
            }
            Text {
                anchors.centerIn: parent
                text: piece
                font.pixelSize: parent.width * 0.9
                font.family: chessFont.name
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
