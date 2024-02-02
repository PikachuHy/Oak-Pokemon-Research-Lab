// Created with Qt Quick Effect Maker (version 0.43), Fri Dec 29 09:05:26 2023

import QtQuick

Item {
    id: rootItem

    // Enable this to animate iTime property
    property bool timeRunning: false
    // When timeRunning is false, this can be used to control iTime manually
    property real animatedTime: frameAnimation.elapsedTime

    // Texture for the fog effect. RGB layers are utilized as a separate fog sprites. This should support seamless tiling.
    property var fogTexture: imageItemfogTexture
    // Scaling of the fog texture.
    property point fogTextureScale: Qt.point(2, 2)
    // Defines how the fog moves on x and y coordinates while it animates. The default value is (0, 0) so not moving and practical range between (-1.0, -1.0) and (1.0, 1.0).
    property point fogMovement: Qt.point(-0.0147911, 0.04)
    // Defines the color of the fog. The default value is light gray (0.8, 0.8, 0.8, 1.0).
    property color fogColor: Qt.rgba(1, 1, 1, 1)
    // Defines the speed how fast the fog frames are animated. The default value is 1.0.
    property real fogAnimationSpeed: 1.16298
    // The amount of snow layers. Increasing the layers adds variation to the snowing effect, but also requires more processing power. The default value is 4 and practical range is between 1 and 10.
    property int snowingLayers: 6
    // Size of a single snowflake. The default value is 1.0 and practical range is between 0.2 and 2.0.
    property real snowingFlakeSize: 0.5
    // Defines gravity of the snowing effect so how fast the snowflakes fall down. The default value is 1.0 and practical range is between 0.2 and 2.0.
    property real snowingGravity: 0.8
    // Defines how much snowflakes positions vary while they are coming down. The default value is 0.5 and the range is between 0.0 and 1.0.
    property real snowingVariation: 1

    FrameAnimation {
        id: frameAnimation
        running: rootItem.timeRunning
    }

    ShaderEffect {
        readonly property alias iTime: rootItem.animatedTime
        readonly property alias fogTexture: rootItem.fogTexture
        readonly property alias fogTextureScale: rootItem.fogTextureScale
        readonly property alias fogMovement: rootItem.fogMovement
        readonly property alias fogColor: rootItem.fogColor
        readonly property alias fogAnimationSpeed: rootItem.fogAnimationSpeed
        readonly property alias snowingLayers: rootItem.snowingLayers
        readonly property alias snowingFlakeSize: rootItem.snowingFlakeSize
        readonly property alias snowingGravity: rootItem.snowingGravity
        readonly property alias snowingVariation: rootItem.snowingVariation

        Image {
            id: imageItemfogTexture
            anchors.fill: parent
            source: "fog.png"
            visible: false
        }

        vertexShader: 'snowyeffect.vert.qsb'
        fragmentShader: 'snowyeffect.frag.qsb'
        anchors.fill: parent
        mesh: GridMesh {
            resolution: Qt.size(16, 16)
        }
    }
}
