import adapter from '@sveltejs/adapter-static';
import { vitePreprocess } from '@sveltejs/vite-plugin-svelte';

/** @type {import('@sveltejs/kit').Config} */
const config = {
	preprocess: vitePreprocess(),

	kit: {
		adapter: adapter({
			fallback: '404.html'
		}),
		prerender: {
			handleMissingId: 'ignore'
		},
		paths: {
			base: process.env.NODE_ENV === 'production' ? '/stelmach-snhu-cs-499' : ''
		},
		alias: {
			'@/*': './src/lib/*'
		}
	}
};

export default config;
