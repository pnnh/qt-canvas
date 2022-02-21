import {QtLoader} from './qtloader'

export function initWasm() {
    var canvas = document.querySelector('#qtcanvas');
    document.addEventListener('DOMContentLoaded', function () {
        console.log('3 seconds passed')
        const qtLoader = QtLoader({
            canvasElements: [canvas],
            //showLoader: () => { },
            showError: function (errorMessage: string) {
                console.log('showError', errorMessage)
            },
            // showExit: function () {
            // },
            // showCanvas: function () {
            // }
        })
        const filePath = '/qt-canvas'
        qtLoader.loadEmscriptenModule(filePath)
    })
}