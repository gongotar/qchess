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
}
