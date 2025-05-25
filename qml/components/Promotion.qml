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

Popup {
    id: promotion
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
            model: promotion.promotionPieces
            delegate: Text {
                text: modelData
                font.pixelSize: parent.width / 4
                font.family: chessFont.name
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        Controller.promotePawnTo(promotion.row,
                                                 promotion.col,
                                                 modelData);
                        promotion.close();
                    }
                }
            }
        }
    }
    Connections {
        target: Controller
        function onPromotePawn(row, col, pieces) {
            promotion.row = row
            promotion.col = col
            promotion.promotionPieces = pieces
            promotion.open()
        }
    }
}
