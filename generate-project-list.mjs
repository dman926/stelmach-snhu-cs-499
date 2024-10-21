import { readdirSync, writeFileSync } from 'fs';

/**
 * @typedef {{
 *  [key: string]: {
 *   name: string;
 *   path: string;
 *   language: 'python' | 'javascript' | 'typescript' | 'markdown' | null;
 *   children: Project
 *  }
 * }} Project
 */

/** @type {Project} */
const projectList = readdirSync('./static/projects', {
	withFileTypes: true,
	encoding: 'utf-8',
	recursive: true
}).reduce((acc, dirent) => {
	if (['__pycache__'].includes(dirent.name) || dirent.name.endsWith('.pyc')) {
		return acc;
	}

	const [projectName, ...name] = dirent.parentPath.split('/').slice(2);
	if (!acc[projectName] && name.length === 0 && projectName !== 'projects') {
		acc[projectName] = {
			name: projectName,
			path: projectName,
			children: {}
		};
	}
	if (name.length > 0) {
		let project = acc[projectName];
		for (let i = 0; i < name.length; i++) {
			if (!project.children[name[i]]) {
				project.children[name[i]] = {
					name: name[i],
					path: `${project.path}/${name[i]}`,
					children: {}
				};
			}
			project = project.children[name[i]];
		}

		let language = null;
		if (dirent.name.endsWith('.ts') || dirent.name.endsWith('.tsx')) {
			language = 'typescript';
		} else if (
			dirent.name.endsWith('.js') ||
			dirent.name.endsWith('.jsx') ||
			dirent.name.endsWith('.cjs') ||
			dirent.name.endsWith('.mjs')
		) {
			language = 'javascript';
		} else if (dirent.name.endsWith('.py') || dirent.name.endsWith('.ipynb')) {
			language = 'python';
		} else if (dirent.name.endsWith('.md')) {
			language = 'markdown';
		} else if (dirent.name.endsWith('.json')) {
			language = 'json';
		} else if (dirent.name.endsWith('.html')) {
			language = 'markup';
		} else if (dirent.name.endsWith('.css') || dirent.name.endsWith('.scss')) {
			language = 'scss';
		} else if (dirent.name.endsWith('.c')) {
			language = 'c';
		}

		project.children[dirent.name] = {
			name: dirent.name,
			path: `${project.path}/${dirent.name}`,
			language,
			children: {}
		};
	}

	return acc;
}, {});

writeFileSync('./static/projectList.json', JSON.stringify(projectList, null, 2), 'utf-8');
