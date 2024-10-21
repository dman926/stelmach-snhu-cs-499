<script lang="ts">
	import { createEventDispatcher } from 'svelte';
	import { type FileEntry, CodeFile } from './code-file';

	export let files: FileEntry;
	export let selectedFile: CodeFile | null;

	const dispatch = createEventDispatcher<{ select: CodeFile }>();
</script>

<ul class="ml-4">
	{#each Object.values(files.children) as file}
		<li>
			{#if file instanceof CodeFile}
				<!-- svelte-ignore a11y-missing-attribute -->
				<a
					on:click={() => {
						dispatch('select', file);
					}}
					on:keydown={(event) => {
						if (event.key === 'Enter' || event.key === ' ') {
							event.preventDefault();
							dispatch('select', file);
						}
					}}
					role="button"
					tabindex="0"
				>
					<span class="underline-hover" class:after:w-full={file.path === selectedFile?.path}>
						{file.name}
					</span>
				</a>
			{:else}
				<span>{file.name}/</span>
				<svelte:self files={file} {selectedFile} on:select={(event) => dispatch('select', event.detail)} />
			{/if}
		</li>
	{/each}
</ul>
