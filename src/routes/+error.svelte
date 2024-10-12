<script>
	import { base } from '$app/paths';
	import { page } from '$app/stores';
</script>

<span class="grid place-content-center gap-4 h-full text-center">
	{#if $page.status == 404}
		<h1>Requested page does not exist</h1>
	{:else}
		<h1>
			{$page.status}{#if $page.error}: {$page.error.message}{/if}
		</h1>
	{/if}
	<a href={`${base}/`} class="text-xl underline-hover">Take me back to civilization</a>
</span>
