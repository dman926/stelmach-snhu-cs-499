<script lang="ts">
	import { base } from '$app/paths';
	import { page } from '$app/stores';

	interface Route {
		href: string;
		title: string;
	}

	const routes: Route[] = [
		{ anchor: 'self-assessment', title: 'Self-Assessment' },
		{ anchor: 'code-review', title: 'Code Review' },
		{ anchor: 'cat-1', title: 'Software Design and Engineering' },
		{ anchor: 'cat-2', title: 'Algorithms and Data Structures' },
		{ anchor: 'cat-3', title: 'Databases' }
	].map(({ anchor, title }) => {
		return {
			title,
			href: `${base}/#${anchor}`
		};
	});
</script>

<nav class="border-r whitespace-nowrap px-4 py-2 grid grid-rows-4">
	<ul class="row-start-2 row-end-4">
		{#each routes as { href, title }, index}
			<li
				class="my-3 text-center underline-hover"
				class:after:w-full={href === `${$page.url.pathname}${$page.url.hash}` ||
					!(index || $page.url.hash)}
			>
				<a {href}>{title}</a>
			</li>
		{/each}
	</ul>
</nav>
