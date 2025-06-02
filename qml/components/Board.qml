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
        Text {
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.margins: 0.03 * parent.width
            text: String.fromCharCode('a'.charCodeAt(0) + col)
            font.pixelSize: parent.width * 0.25
            color: (row + col) % 2 === 0 ? "darkgoldenrod":"white"
            visible: row == 7
        }
        Text {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 0.03 * parent.width
            font.pixelSize: parent.width * 0.25
            color: (row + col) % 2 === 0 ? "darkgoldenrod":"white"
            text: Controller.flipped ? (row + 1) : (8 - row)
            visible: col == 7
        }
    }
}
