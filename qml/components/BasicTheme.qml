import QtQuick

Item {
    id: basicTheme

    property string lightSquare         : "white"
    property string darkSquare          : "darkgoldenrod"

    // highlights
    property string highlightColor      : "gold"
    property real  highlightOpac        : 0.22
    property string selectColor         : "blue"
    property real  selectOpac           : 0.18
    property string legalDestColor      : "black"
    property real  legalDestOpac        : 0.5

    // board BoardContainer
    property string containerColor      : "#b09030"
    property string borderColor         : "#404020"

    // font
    FontLoader {
        id: chessFont
        source: "qrc:/qt/qml/qchess/resources/Alpha.ttf"
    }
    property string chessFont: chessFont.name
}
