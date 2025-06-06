import QtQuick

Rectangle {
    id: boardContainer
    anchors.fill: parent
    color: theme.containerColor
    property int borderMargin: 10
    property int spacing: 4
    property real boardSize: Math.min(
        boardContainer.width  - borderMargin*2,
        boardContainer.height - borderMargin*2
            - topCaptures.implicitHeight - bottomCaptures.implicitHeight
            - spacing*2
    )

    Text {
        id: topCaptures
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: borderMargin
        horizontalAlignment: Text.AlignHCenter
        text: Controller.flipped ? Controller.whiteCaptures
                                  : Controller.blackCaptures
        font.pixelSize: boardBorder.width / 8 * 0.8
        font.family: theme.chessFont
    }

    Rectangle {
        id: boardBorder
        width: boardSize
        height: boardSize
        color: theme.borderColor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: topCaptures.bottom
        anchors.topMargin: spacing
        Item {
            id: boardWrapper
            anchors.fill: parent
            anchors.margins: 1
            Board {}
        }
    }

    Text {
        id: bottomCaptures
        anchors.top: boardBorder.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: spacing
        anchors.bottom: parent.bottom
        anchors.bottomMargin: borderMargin
        horizontalAlignment: Text.AlignHCenter
        text: Controller.flipped ? Controller.blackCaptures
                                 : Controller.whiteCaptures
        font.pixelSize: boardBorder.width / 8 * 0.8
        font.family: theme.chessFont
    }
}
