import { browser } from '$app/environment';
import { base } from '$app/paths';
import Prism from 'prismjs';
import 'prismjs/components/prism-python';
import 'prismjs/components/prism-javascript';
import 'prismjs/components/prism-typescript';
import 'prismjs/components/prism-markup';
import 'prismjs/components/prism-markdown';
import 'prismjs/components/prism-scss';
import 'prismjs/components/prism-c';

type Language = 'python' | 'javascript' | 'typescript' | 'markup' | 'scss' | 'markdown' | 'json' | 'c';

export class CodeFile {
	public name: string;
	public path: string;
	public language: Language;
	private content: string | undefined;
	private error: boolean;

	constructor(name: string, path: string, language: Language) {
		this.name = name;
		this.path = path;
		this.language = language;
		this.error = false;
	}

	public async getContent(): Promise<string> {
		// Try to get if not already loaded or retry if error
		if (!this.content || this.error) {
			try {
				const fetchContent = await fetch(`/projects/${this.path}`).then((res) => res.text());

				this.content = Prism.highlight(fetchContent, Prism.languages[this.language], this.language);
			} catch (e) {
				console.error(e);
				this.error = true;
				this.content = 'Error fetching file';
			}
		}
		return this.content;
	}

	public get nicePath(): string {
		return this.path.split('/').slice(1).join('/');
	}
}

export interface FileEntry {
	name: string;
	path: string;
	language: Language | null;
	children: { [key: string]: FileEntry | CodeFile };
}

export interface Project {
	[key: string]: FileEntry | CodeFile;
}

function transformToCodeFiles(obj: Project): void {
	for (const key in obj) {
		const entry = obj[key];

		if (entry instanceof CodeFile) continue;

		if (entry.language) {
			obj[key] = new CodeFile(entry.name, entry.path, entry.language);
		}

		if (entry.children && Object.keys(entry.children).length > 0) {
			transformToCodeFiles(entry.children);
		}
	}
}

export const files = browser
	? fetch(`${base}/projectList.json`)
			.then((res) => res.json())
			.then((res) => {
				transformToCodeFiles(res);
				return res;
			})
	: Promise.resolve({});
