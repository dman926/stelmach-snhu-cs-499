<script lang="ts">
	import { base } from '$app/paths';
	import { mode } from 'mode-watcher';

	import TravlrHeader from '@/components/headers/travlr-header.svelte';
	import CodeViewer from '@/components/code-viewer.svelte';
	import { files } from '@/components/code-file';
</script>

<!-- TODO -->
<h1 class="text-2xl text-center">Databases</h1>
<TravlrHeader i={3} />
<p>
	&emsp;The artifact was enhanced by expanding the database's role. Previously, the database only
	stored the basic trip and admin information. The database was expanded to store more information
	about the application itself to allow for a more dynamic and user-friendly method for website
	content editing by non-programmers. The image below shows the expanded database using an
	entity-relationship diagram.
</p>
<div class="rounded-xl border-2 border-primary overflow-hidden w-fit m-auto">
	<img
		src={`${base}Travlr Entity-Relation-Diagram-${$mode ?? 'light'}.png`}
		alt="travlr entity relation diagram"
		width="1128"
	/>
</div>
<p>
	&emsp;The artifact was also enhanced by changing the database from MongoDB, a NoSQL solution, to
	Postgres, an SQL solution. I made this change because the application's data is very normalized,
	with well defined types and clear boundaries between each entity, and is inherently relational,
	making a relational database a much more ideal choice. This change also enforces type safety, as
	an SQL database will require well-defined types and formats for data, whereas a NoSQL database
	will be much more flexible, allowing for essentially anything to be stored, even non-structured
	data, which may become an issue if an admin user adds non-expected data to the database.
</p>
{#await files then { 'travlr': travlrFiles }}
	<CodeViewer files={travlrFiles} />
{/await}