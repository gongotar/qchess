import QtQuick

GridView {
    id: board
    anchors.fill: parent
    anchors.margins: 10
    cellWidth: width / 8
    cellHeight: height / 8
    model: Controller.board
    delegate: Rectangle {
        width: board.width / 8
        height: board.height / 8
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
                Controller.selectOrMovePiece(row, col)
            }
        }
    }
}
