module.exports = {

    router: {
        axes: require('./router/axes')
    },

    Controllers: {
        DrawLeg: require('./functions/drawLeg')
    },

    Algorithmes: {
        inverseKinematic: require('./algorithmes/inverseKinematic')
    }

}