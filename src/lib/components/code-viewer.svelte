<script lang="ts">
	import CodeViewerNav from './code-viewer-nav.svelte';
	import { type CodeFile, type FileEntry } from './code-file';
	import { writable } from 'svelte/store';

	export let files: FileEntry;
	const selectedFile = writable<CodeFile | null>(null);
</script>

<div class="flex border-primary rounded-xl border-2 max-h-[calc(100vh-12rem)]">
	<nav class="border-r whitespace-nowrap overflow-y-auto">
		<CodeViewerNav
			{files}
			selectedFile={$selectedFile}
			on:select={(event) => selectedFile.set(event.detail)}
		/>
	</nav>
	<article class="flex-1 overflow-auto">
		{#if $selectedFile}
			{#await $selectedFile.getContent() then content}
				<div class="whitespace-pre">
					{@html content}
				</div>
			{/await}
		{:else}
			<p>No file selected</p>
		{/if}
	</article>
</div>
