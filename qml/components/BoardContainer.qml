import QtQuick

Rectangle {
    id: boardContainer
    anchors.fill: parent
    color: theme.containerColor
    property int borderMargin: 10
    property real borderSize: Math.min(
        boardContainer.width  - borderMargin*2,
        boardContainer.height - borderMargin*2
    )

    Text {
        id: topCaptures
        anchors.bottom: boardBorder.top
        anchors.left: boardBorder.left
        anchors.right: boardBorder.right
        anchors.bottomMargin: 4
        horizontalAlignment: Text.AlignHCenter
        text: Controller.flipped ? Controller.whiteCaptures
                                  : Controller.blackCaptures
        font.pixelSize: boardBorder.width / 8 * 0.8
        font.family: theme.chessFont
    }

    Rectangle {
        id: boardBorder
        x: borderMargin
        y: borderMargin
        width:  borderSize
        height: borderSize
        color: theme.borderColor
        Item {
            id: boardWrapper
            anchors.fill: boardBorder
            anchors.margins: 1
            Board {}
        }
    }

    Text {
        id: bottomCaptures
        anchors.top: boardBorder.bottom
        anchors.left: boardBorder.left
        anchors.right: boardBorder.right
        anchors.topMargin: 4
        horizontalAlignment: Text.AlignHCenter
        text: Controller.flipped ? Controller.blackCaptures
                                 : Controller.whiteCaptures
        font.pixelSize: boardBorder.width / 8 * 0.8
        font.family: theme.chessFont
    }
}
