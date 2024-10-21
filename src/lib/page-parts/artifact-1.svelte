<script lang="ts">
	import { base } from '$app/paths';

	import TravlrHeader from '@/components/headers/travlr-header.svelte';
	import CodeViewer from '@/components/code-viewer.svelte';
	import { files } from '@/components/code-file';
</script>

<h1 class="text-2xl text-center">Software Design and Engineering</h1>
<h2 class="text-xl text-center underline" id="animal-shelter">
	Artifact 1: Animal Shelter Library and Web Dashboard
</h2>
<img
	class="mx-auto"
	src={`${base}/Grazioso Salvare Logo.png`}
	alt="Grazioso Salvare Logo"
	width="200"
/>
<p>
	&emsp;This artifact is a simple Python create/read/update/delete (CRUD) library for MongoDB
	wrapped with a Jupyter notebook web dashboard. The web dashboard did not include any
	authentication or security measures, and the entire project was housed in a single Python and
	Jupyter notebook.
</p>
{#await files then { 'animal-shelter': animalShelterFiles }}
	<CodeViewer files={animalShelterFiles} />
{/await}
<p>
	&emsp;The artifact was enhanced by recreating the application with the Flask framework. This
	refactor split up the files into its respective directories and files to improve organization.
	This makes future development and debugging easier by providing smaller and more manageable files
	instead of the original large and unweildy Jupyter notebook.
</p>
<p>
	&emsp;The artifact was also enhanced by adding authentication capabilities to the web dashboard.
	This prevents unauthorized users from modifying the database. This is important because the
	artifact is a web-based application, and so will be accessible to anyone with an internet
	connection, so it is important to authenticate the user before allowing them access to
	administrator-level functions.
</p>
<p>
	&emsp;Finally, the artifact was enhanced by changing from a MongoDB database to an SQL database. I
	used SQLAlchemy as the object-relational mapper (ORM) to simplify the database query code. I made
	this change because the data that exists in this application is well-defined with consistent
	schemas, and so can be easily used within a table-based SQL database, which generally produces
	faster query capabilities.
</p>
<br />
<TravlrHeader i={1} />
<p>
	&emsp;This artifact is a MongoDB/Express/Angular/Node (MEAN) web application to facilitate a
	travel agency. The project was split into three sections, with two different webservers running,
	one for Express (API layer) and one for the Angular application (UI layer).
</p>
<p>
	&emsp;The artifact was enhanced by recreating the application with the SvelteKit framework. This
	refactor allows for several things. Most importantly, it allows for a single running webserver.
	This provides a single application to run, making deployments and testing of the application more
	efficient. Another important feature is server-side rendering. A major downfall of single-page
	applications is the lack of server-side rendering. The lack of server-side rendering makes it more
	difficult for web-crawlers, such as those from search engines, to crawl the web application to
	provide meaningful data, which is detrimental to search engine optimization. By using server-side
	rendering, the web application can be indexed by search engines, allowing for more meaningful
	search results, enhancing the applications visibility.
</p>
