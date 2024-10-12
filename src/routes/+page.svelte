<script>
	import Artifact_1 from '@/page-parts/artifact-1.svelte';
	import Artifact_2 from '@/page-parts/artifact-2.svelte';
	import Artifact_3 from '@/page-parts/artifact-3.svelte';
	import CodeReview from '@/page-parts/code-review.svelte';
	import SelfAssessment from '@/page-parts/self-assessment.svelte';
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
	<Artifact_1 />
</section>
<hr />
<section id="cat-2">
	<Artifact_2 />
</section>
<hr />
<section id="cat-3">
	<Artifact_3 />
</section>

<style>
	section {
		padding: 0.5rem;
	}
</style>
