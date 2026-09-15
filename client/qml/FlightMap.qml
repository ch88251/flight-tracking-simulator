import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning

Item {
    id: root

    Plugin {
        id: osmPlugin
        name: "osm"

        // Qt's OSM plugin normally proxies through a providers repository that
        // defaults to Thunderforest styles requiring an API key. Point it at
        // plain OpenStreetMap tiles directly so no key is needed.
        PluginParameter { name: "osm.mapping.providersrepository.disabled"; value: true }
        PluginParameter { name: "osm.mapping.host"; value: "https://tile.openstreetmap.org/" }
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: osmPlugin
        center: QtPositioning.coordinate(39.5, -98.35) // Continental US
        zoomLevel: 4

        Component.onCompleted: {
            for (let i = 0; i < supportedMapTypes.length; ++i) {
                if (supportedMapTypes[i].style === MapType.StreetMap) {
                    activeMapType = supportedMapTypes[i]
                    break
                }
            }
        }

        MapItemView {
            model: flightModel
            delegate: MapQuickItem {
                coordinate: QtPositioning.coordinate(model.latitude, model.longitude)
                anchorPoint: Qt.point(marker.width / 2, marker.height / 2)

                sourceItem: Column {
                    spacing: 2

                    Text {
                        id: marker
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "✈"
                        font.pixelSize: 22
                        // The glyph's default orientation points east (90°), not north,
                        // so headings must be offset by -90 to align with compass bearing.
                        rotation: model.headingDeg - 90
                        transformOrigin: Item.Center

                        MouseArea {
                            anchors.fill: parent
                            anchors.margins: -6
                            onClicked: infoBubble.visible = !infoBubble.visible
                        }
                    }

                    Rectangle {
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: label.width + 8
                        height: label.height + 4
                        radius: 3
                        color: "#cc1e1e1e"

                        Text {
                            id: label
                            anchors.centerIn: parent
                            text: model.callsign
                            color: "white"
                            font.pixelSize: 11
                        }
                    }

                    Rectangle {
                        id: infoBubble
                        visible: false
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: infoText.width + 12
                        height: infoText.height + 8
                        radius: 4
                        color: "#eeffffff"
                        border.color: "#888888"

                        Text {
                            id: infoText
                            anchors.centerIn: parent
                            text: model.origin + " → " + model.destination
                                  + "\n" + model.status
                                  + "\n" + model.altitudeFt.toFixed(0) + " ft, "
                                  + model.groundSpeedKts.toFixed(0) + " kt"
                            color: "#111111"
                            font.pixelSize: 10
                        }
                    }
                }
            }
        }

        WheelHandler {
            acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
            onWheel: (event) => {
                const zoomDelta = event.angleDelta.y / 120
                map.zoomLevel = Math.max(map.minimumZoomLevel,
                                          Math.min(map.maximumZoomLevel, map.zoomLevel + zoomDelta))
            }
        }

        DragHandler {
            id: dragHandler
            target: null
            onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
        }
    }

    Column {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 8
        spacing: 4

        Button {
            text: "+"
            onClicked: map.zoomLevel = Math.min(map.maximumZoomLevel, map.zoomLevel + 1)
        }
        Button {
            text: "−"
            onClicked: map.zoomLevel = Math.max(map.minimumZoomLevel, map.zoomLevel - 1)
        }
    }
}
