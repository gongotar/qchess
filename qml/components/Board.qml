import QtQuick

GridView {
    id: board
    anchors.fill: parent
    cellWidth: width / 8
    cellHeight: height / 8
    model: Controller.board
    delegate: Rectangle {
        width: board.width / 8
        height: board.height / 8
        color: (row + col) % 2 === 0 ? theme.lightSquare : theme.darkSquare
        Rectangle {
            visible: highlight
            anchors.fill: parent
            color: theme.highlightColor
            opacity: theme.highlightOpac
        }
        Rectangle {
            visible: isSelected
            anchors.fill: parent
            color: theme.selectColor
            opacity: theme.selectOpac
        }
        Rectangle {
            visible: isLegalDestination
            anchors.centerIn: parent
            width: parent.width * 0.4
            height: parent.height * 0.4
            radius: width / 2
            color: theme.legalDestColor
            opacity: theme.legalDestOpac
        }
        Text {
            anchors.centerIn: parent
            text: piece
            font.pixelSize: parent.width * 0.9
            font.family: theme.chessFont
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
            color: (row + col) % 2 === 0 ? theme.darkSquare:theme.lightSquare
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
