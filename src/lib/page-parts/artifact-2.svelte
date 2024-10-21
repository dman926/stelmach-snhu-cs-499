<script lang="ts">
	import { base } from '$app/paths';

	import TravlrHeader from '@/components/headers/travlr-header.svelte';
	import CodeViewer from '@/components/code-viewer.svelte';
	import { files } from '@/components/code-file';
</script>

<!-- TODO -->
<h1 class="text-2xl text-center">Algorithms and Data Structures</h1>
<h2 class="text-xl text-center underline" id="embedded-thermostat">
	Artifact 2: Embedded Thermostat
</h2>
<!-- TODO: Add code blocks -->
<p>
	&emsp;This artifact is a thermostat program written in a C variant for an embedded
	microcontroller. The program utilizes state machines to control the set temperature and heating
	status.
</p>
<p>
	&emsp;The artifact was enhanced by simplifying the state machines. This makes the program more
	readable and easier to step through. The set temperature state machine was also enhanced to
	include a lerp function. This allows the user to press and hold the up or down set temperature
	button and have the set temperature change smoothly with increasing speed.
</p>
<p>
	&emsp;The artifact was also enhanced by providing a safety mechanism to lock out the heat calling
	functionality when the recorded temperature is too high, either from a bad sensor or unsafe user
	input. This is important because without it, the thermostat may incorrectly call for heat up to a
	dangerous level and potentially start a fire.
</p>
<div class="flex gap-2 my-4">
	<div class="flex flex-col justify-center">
		<img
			class="mx-auto rounded-xl border-primary"
			width="800"
			src={`${base}/Stelmach_CC3220S_Thermostat_v1_State_Diagram.png`}
			alt="Embedded Thermstat V1 State Diagram"
		/>
		<caption>
			Embedded Thermstat V1 State Diagram
			<br />
			This diagram is overly complex, and features bidirectional transitions for each state, which adds
			to the complexity.
		</caption>
	</div>
	<div class="flex flex-col justify-center">
		<img
			class="mx-auto rounded-xl border-primary"
			width="800"
			src={`${base}/Stelmach_CC3220S_Thermostat_v2_State_Diagram.png`}
			alt="Embedded Thermstat V2 State Diagram"
		/>
		<caption class="text-center">
			Embedded Thermstat V2 State Diagram
			<br />
			The simplified manner shows the each state with a dedicated input and output.
		</caption>
	</div>
</div>
{#await files then { 'embedded-thermostat': embeddedThermostatFiles }}
	<CodeViewer files={embeddedThermostatFiles} />
{/await}
<p>
	&emsp;The inclusion of the lerp algorithm provides a much better user experience. With this, the
	user can easily move up or down with a button press, while still retaining fine controls. The
	previous implementation allowed for a button hold to increase and decrease the temperature, but
	its increment/decrement speed was locked at the speed of the state machine. By moving the
	functionality of the set temperature out of the main state machine, we can have the microprocessor
	focus on the heating functionality, while only referencing the set temperature when needed instead
	of dedicating the state machine to check the current increment/decrement state and handle it as
	well.
</p>
<p>
	&emsp;This is especially useful when using new-age microprocessors that include mutliple cores.
	This paradigm allows for the microprocessor to dedicate an entire core to a certain task. In this
	case, one core would focus on the set temperature and user interface while another core is
	dedicated to checking and calling for heat when needed.
</p>
<br />
<TravlrHeader i={2} />
<!-- TODO: Add code blocks and text -->
<p>
	&emsp;As discussed in <a class="underline" href={`${base}/#travlr-1`}>the previous section</a>, the Travlr application
	was enhanced with server-side rendering by using SvelteKit. This paradigm shift allows for search engine web-crawlers
	to get accurate information for SEO and provides faster time-to-active timings for initial page
	loads. This enhancement showcases my ability to use computer science principles to make the
	program work faster and more efficiently.
</p>
<p>
	&emsp;The artifact was also enhanced by moving security responsibilities, which utilizes JSON Web
	Tokens, to the application's middleware. This allows for a seamless and secure method for
	providing access control rules by checking the validity of the token for every request, and
	allowing the server to prevent unauthorized access, as client controlled access control is a major
	security concern.
</p>
