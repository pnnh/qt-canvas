import {defineConfig} from 'vite'
import path from 'path'

const config = defineConfig({
    base: '/',
    server: {
        hmr: true
    },
    resolve: {
        alias: [
            {find: '@', replacement: path.resolve(__dirname, 'src')},
            {find: '~', replacement: path.resolve(__dirname, 'node_modules')}
        ]
    }
})

export default config
