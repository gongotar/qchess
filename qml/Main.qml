// MIT License
//
// Copyright (c) 2025 Masoud Jami
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

import QtQuick
import QtQuick.Controls
import qchess

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
        model: Controller.board
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
                    Controller.selectOrMovePiece(row, col)
                }
            }
        }
    }

    Popup {
        id: promotionPopup
        modal: true
        focus: true
        anchors.centerIn: parent
        height: parent.height / 7
        width: parent.width / 2
        closePolicy: Popup.NoAutoClose

        property int row
        property int col;
        property var promotionPieces;

        background: Rectangle {
            color: "darkgoldenrod"
            radius: 8
        }

        contentItem: Row {
            anchors.centerIn: parent

            // You can show icons or letters for pieces
            Repeater {
                model: promotionPopup.promotionPieces
                delegate: Text {
                    text: modelData
                    font.pixelSize: parent.width / 4
                    font.family: chessFont.name
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            Controller.promotePawnTo(promotionPopup.row,
                                                     promotionPopup.col,
                                                     modelData);
                            promotionPopup.close();
                        }
                    }
                }
            }
        }
    }
    Connections {
        target: Controller
        function onPromotePawn(row, col, pieces) {
            promotionPopup.row = row
            promotionPopup.col = col
            promotionPopup.promotionPieces = pieces
            promotionPopup.open()
        }
    }
}
