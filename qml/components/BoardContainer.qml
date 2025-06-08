import QtQuick

Rectangle {
    id: boardContainer
    anchors.fill: parent
    color: theme.containerColor
    property int borderMargin: 10
    property int spacing: 4
    property real boardSize: boardContainer.width - borderMargin*2
    property real captureSize: boardSize / 8 * 0.8

    Row {
        id: topCaptures
        anchors.top: parent.top
        anchors.left: boardBorder.left
        anchors.topMargin: borderMargin
        spacing: 2
        height: captureSize
        Text {
            text: Controller.flipped ? Controller.whiteCapturePieces
                                     : Controller.blackCapturePieces
            font.pixelSize: captureSize
            font.family: theme.chessFont
        }
        Text {
            text: {
                const v = Controller.flipped ? Controller.whiteCaptureValue
                                             : Controller.blackCaptureValue;
                return v > 0 ? "+" + v : "";
            }
            font.pixelSize: captureSize
        }
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

    Row {
        id: bottomCaptures
        anchors.top: boardBorder.bottom
        anchors.right: boardBorder.right
        anchors.topMargin: spacing
        anchors.bottom: parent.bottom
        anchors.bottomMargin: borderMargin
        spacing: 2
        height: captureSize
        Text {
            text: Controller.flipped ? Controller.blackCapturePieces
                                     : Controller.whiteCapturePieces
            font.pixelSize: captureSize
            font.family: theme.chessFont
        }
        Text {
            text: {
                const v = Controller.flipped ? Controller.blackCaptureValue
                                             : Controller.whiteCaptureValue;
                return v > 0 ? "+" + v : "";
            }
            font.pixelSize: captureSize
        }
    }
}
