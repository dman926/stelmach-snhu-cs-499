<script>
	import { toggleMode } from 'mode-watcher';
	import { Button } from '$lib/components/ui/button';
</script>

<Button on:click={toggleMode} variant="outline" size="icon" class="light-switch">
	<span
		class="iconify noto--sun rotate-0 scale-100 transition-all dark:-rotate-90 dark:scale-0"
	></span>

	<span
		class="iconify noto--crescent-moon absolute rotate-90 scale-0 transition-all dark:rotate-0 dark:scale-100"
	></span>
	<span class="sr-only">Toggle theme</span>
</Button>
