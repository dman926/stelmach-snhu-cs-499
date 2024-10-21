<script>
	import { base } from '$app/paths';

	import Prism from 'prismjs';
	import 'prismjs/components/prism-markdown';
</script>

<h1 class="text-2xl text-center">Overall Code Review</h1>
<div class="max-w-5xl my-4 rounded-xl border-2 border-primary overflow-hidden w-fit mx-auto">
	<!-- svelte-ignore a11y-media-has-caption -->
	<video width="1128" height="752" controls>
		<source src={`${base}/Stelmach CS 499 Module 2 Milestone 1 Code Review.mp4`} type="video/mp4" />
	</video>
</div>
<p>
	&emps;This code review steps through each of the artifacts below to provide an overview of the
	artifacts' code and how it was planned to enhance them. The outcome of the code review yielded an
	initial plan for the next iteration each of the projects.
</p>
<ol class="list-decimal list-inside pl-4 max-w-6xl mx-auto">
	<li>
		<a class="underline" href={`${base}/#cat-1`}>
			Artifact 1: Animal Shelter Library and Web Dashboard
		</a>
		<a
			class="underline"
			href="https://github.com/dman926/stelmach-snhu-cs-499/tree/main/static/projects/animal-shelter"
			>View code on GitHub</a
		>
		<div class="mx-auto my-4 max-w-5xl whitespace-pre-wrap rounded-xl border-2 border-primary">
			{@html Prism.highlight(
				`### Review Items

- Use of a Jupyter notebook for the web server creates a long and unweildy program
- Backend handling of client map presents performance and responsiveness issues, as performing these actions directly in the client will produce a much faster and smoother result.
- There are no authentication or security measures in the application. Users are able to create, read, update, and delete any animal DB entry without any recording of the action.
- Non-descript method names create a confusing development environment.

### Enhancement

- Re-creation using an enterprise prefered framework, such as Flask or Django
  - This allows for ease of future development by refactoring the existing code
- The application will include ACL capabilities to secure against unwanted users
- The moving of responsibilities for the client map will produce much faster and smoother results for end users`,
				Prism.languages.markdown,
				'markdown'
			)}
		</div>
	</li>
	<li>
		<a class="underline" href={`${base}/#cat-2`}>Artifact 2: Embedded Thermostat</a>
		<a
			class="underline"
			href="https://github.com/dman926/stelmach-snhu-cs-499/tree/main/static/projects/embedded-thermostat"
			>View code on GitHub</a
		>
		<div class="mx-auto my-4 max-w-5xl whitespace-pre-wrap rounded-xl border-2 border-primary">
			{@html Prism.highlight(
				`### Review Items

- Lock in for TI CC3220S platform of embedded controllers
- Poorly named variables and lack of clarifying comments
- The two complex state machines can be simplified into a single state machine
- Lack of organization and use of a single file presents future development headaches

### Enhancement

- Conversion to the Arduino platform allows for many embedded microcontrollers to be used, with a simplified programming structure via PlatformIO.
- Refactoring of the existing program during conversion allows for better naming of variables and clarifying comments
- Merging of state machines simplifies the overall program
- Addition of software lockout for heating is important for device and user safety`,
				Prism.languages.markdown,
				'markdown'
			)}
		</div>

		<p class="mx-8">
			I decided not to rework the project to a new platform due to the lack of time and resources at
			the time.
		</p>
	</li>
	<li>
		Artifact 3: Travlr Web Application
		<ul class="list-disc list-inside pl-4">
			<li>
				<a class="underline" href={`${base}/#travlr-1`}>Software Design and Engineering </a>
			</li>
			<li>
				<a class="underline" href={`${base}/#travlr-2`}>Algorithms and Data Structures </a>
			</li>
			<li>
				<a class="underline" href={`${base}/#travlr-3`}>Databases</a>
			</li>
		</ul>
		<a
			class="underline"
			href="https://github.com/dman926/stelmach-snhu-cs-499/tree/main/static/projects/travlr"
			>View code on GitHub</a
		>

		<div class="mx-auto my-4 max-w-5xl whitespace-pre-wrap rounded-xl border-2 border-primary">
			{@html Prism.highlight(
				`### Review Items

- Several apps split up
  - Express in CommonJS
  - Angular in TypeScript
  - Lack of shared code
    - Redefinition of ORM
  - Lack shared server
- Poor organization
  - Three top level folders (app_api and app_server is the express project, app_admin is the angular project)
- Unused code
  - public/ folder defines several HTML pages that are never used
- Lack of scaffolding utilities
- Lack of value checking on Creates and Updates to database items allows for malformed or invalid data to be displayed

### Enhancement

- Combining the separated apps allows for
  - a single dev server to be used
  - simple sharing of shared code
- Refactoring the newly combined MEAN application allows for better organization of files, as well as sharing the same development ecosystem, in this case TypeScript
- Removal of unused code simplifies the overall project, and potentially saves on storage costs depending on the final build
- Addition of project scaffolding allows new developers to get started working quickly
- Addition of value checking on Creates and Updates prevents malformed or invalid data to be displayed or used in calculations`,
				Prism.languages.markdown,
				'markdown'
			)}
		</div>
	</li>
</ol>
