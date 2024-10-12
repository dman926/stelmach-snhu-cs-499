<script>
	import CodeReview from '@/page-parts/code-review.svelte';
	import Intro from '@/page-parts/intro.svelte';
</script>

<section id="intro">
	<Intro />
</section>
<hr />
<section id="code-review">
	<CodeReview />
</section>
<hr />
<section id="cat-1">
	<p>Cat 1</p>
	<Intro />
</section>
<hr />
<section id="cat-2">
	<p>Cat 2</p>
	<Intro />
</section>
<hr />
<section id="cat-3">
	<p>Cat 3</p>
	<Intro />
</section>

<style>
	section {
		padding: 0.5rem;
	}
</style>
