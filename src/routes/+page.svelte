<script>
	import SelfAssessment from '@/page-parts/self-assessment.svelte';
	import CodeReview from '@/page-parts/code-review.svelte';

	const artifact_1 = import('@/page-parts/artifact-1.svelte');
	const artifact_2 = import('@/page-parts/artifact-2.svelte');
	const artifact_3 = import('@/page-parts/artifact-3.svelte');
</script>

<span>
	<h1 class="text-3xl text-center">SNHU CS Capstone Portfolio</h1>
	<h2 class="text-2xl text-center">Dennis James Stelmach</h2>
</span>
<hr />
<section id="self-assessment">
	<SelfAssessment />
</section>
<hr />
<section id="code-review">
	<CodeReview />
</section>
<hr />
<section id="cat-1">
	{#await artifact_1 then { default: Artifact_1 }}
		<Artifact_1 />
	{/await}
</section>
<hr />
<section id="cat-2">
	{#await artifact_2 then { default: Artifact_2 }}
		<Artifact_2 />
	{/await}
</section>
<hr />
<section id="cat-3">
	{#await artifact_3 then { default: Artifact_3 }}
		<Artifact_3 />
	{/await}
</section>

<style>
	section {
		padding: 0.5rem;
	}
</style>
