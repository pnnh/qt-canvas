import ReactDOM from 'react-dom'
import React from 'react'
import {EditPage} from "./editor"
import {initWasm} from './qtloader/init'

function App() {
    return <EditPage></EditPage>
}

const rootElement = document.getElementById('root')
if (rootElement) {
    ReactDOM.render(<App/>, rootElement)
}

initWasm()