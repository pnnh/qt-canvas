import React, {useState} from 'react'
import type {SFEditorModel} from '@pnnh/stele'
import {SFXEditor} from '@pnnh/stele'

const initialValue = {
    children: [{
        name: 'paragraph',
        children: [{name: 'text', text: ''}]
    }]
}

export function EditPage(props: {}, state: {}) {
    console.debug('EditPage')
    const [editorValue, setEditorValue] = useState<SFEditorModel>(initialValue)
    const [markdown, setMarkdown] = useState<string>('')

    return <div className={'article-edit-page'}>
        <div className={'article-edit'}>
            <div className={'markdown-area'}>
                <textarea value={markdown} cols={80} rows={20} onChange={(event) => {
                    console.debug('Markdown变化', markdown.length)
                    setMarkdown(event.target.value)
                }}></textarea>
            </div>
            <div className={'submit-area'}>
                <div>
                    <button className={'fx-primary-button'} onClick={() => {
                        console.debug('开始转换\n', markdown)
                        const md2json = window.Module.tryMarkdown2Json(markdown)
                        console.debug('转换结果\n', md2json)
                    }}>
                        转换
                    </button>
                </div>
            </div>
            <div className={'body-area'}>
                <div>
                    <SFXEditor value={editorValue} onChange={(value) => {
                        console.debug('onChange222')
                        setEditorValue(value)
                    }}/>
                </div>
            </div>
            <div className={'submit-area'}>
                <div>
                    <button className={'fx-primary-button'} onClick={() => {
                        console.debug('打印内容', editorValue)
                    }}>
                        打印内容
                    </button>
                </div>
            </div>
        </div>
    </div>
}
