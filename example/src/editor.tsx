import React, {useState} from 'react'

const initialValue = {
    children: [{
        name: 'paragraph',
        children: [{name: 'text', text: ''}]
    }]
}

export function EditPage(props: {}, state: {}) {
    console.debug('EditPage')
    const [editorValue, setEditorValue] = useState(initialValue)
    const [markdown, setMarkdown] = useState<string>('')

    return <div className={'article-edit-page'}>
        <div className={'article-edit'}>
            <div className={'markdown-area'}>
                <textarea value={markdown} cols={80} rows={10} onChange={(event) => {
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
                        const jsonValue = JSON.parse(md2json)
                        if (jsonValue) {
//                            setEditorValue(jsonValue)
                            setEditorValue({
                                children: [{
                                    name: 'paragraph',
                                    children: [{name: 'text', text: 'qqqqq'}]
                                }]
                            })
                        }
                    }}>
                        转换
                    </button>
                </div>
            </div>
            <div className={'body-area'}>
                <div>
                    编辑器
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
