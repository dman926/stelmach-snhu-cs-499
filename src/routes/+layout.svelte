<script>
	import { page } from '$app/stores';
	import { ModeWatcher } from 'mode-watcher';
	import LightSwitch from '@/components/light-switch.svelte';
	import SideNav from '@/components/side-nav.svelte';
	import '../app.pcss';
</script>

<svelte:head>
	<title>Stelmach 499 Portfolio</title>
</svelte:head>

<ModeWatcher />

<LightSwitch />

<div class="min-h-dvh flex fixed top-0 left-0 w-full h-full overflow-y-auto">
	<!-- Only show the side nav if this is not an error page -->
	{#if !$page.error}
		<SideNav />
	{/if}
	<main class="flex flex-col flex-1 overflow-y-auto px-4 py-1">
		<slot />
	</main>
</div>
