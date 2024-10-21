<script lang="ts">
	import Prism from 'prismjs';
	import 'prismjs/components/prism-typescript';
</script>

<h1 class="text-2xl text-center">Self Assessment</h1>
<p>
	&emsp;Completing the computer science program and capstone has helped me in a number of ways. The
	program has shown me a wide variety of fields in computer science and has strengthened my
	understanding of software and methodologies. I have been working as a software engineer throughout
	my college career, and completing the course has given me various methods to solve problems in my
	professional life.
</p>
<p>
	&emsp;This capstone has showcased my skills to collaborate in a team environment by reorganizing
	each project and commenting appropriately in order to help the developers that come next. In my
	professional experience, there is nothing worse than inheriting a legacy project that is severely
	disorganized and not developer friendly.
</p>
<p>
	&emsp;It has also shown my skills with data structures and algorithms by showcasing a number of
	ways to increase application performance and use complex data structures. As an example, this
	website uses a complex recursive type to facilitate the in-site code browser:
</p>
<div class="whitespace-pre border-2 border-primary rounded-xl p-4 mx-auto my-4 max-w-96">
	{@html Prism.highlight(
		`type Language = 
			| 'python'
			| 'javascript'
			| 'typescript'
			| 'markup'
			| 'scss'
			| 'markdown'
			| 'json'
			| 'c';
		
interface CodeFile {
	name: string;
	path: string;
	language: Language;
}
	
interface FileEntry {
	name: string;
	path: string;
	language: Language | null;
	children: { [key: string]: FileEntry | CodeFile };
}
	
interface Project {
	[key: string]: FileEntry | CodeFile;
}`,
		Prism.languages.markup,
		'markup'
	)}
</div>
<p>
	&emsp;Another example is the restructuring of the state machines in the embedded thermostat. This
	was a complex state machine that I had previously lacked to document thoroughly enough. Unwrapping
	and recreating the state machine into a simpler format was a large task, but I was able to achieve
	it.
</p>
<p>
	&emsp;The capstone showcases my ability in software engineering, databases, and security by
	restructuring the Animal Shelter and Travlr web applications into more performant versions of
	their previous selves. Particularly with the Travlr web application, I had implemented server-side
	rendering and a highly performant meta-framework. This is extremely useful for a commercial web
	application like Travlr in order to allow search engine web-crawlers to properly gather
	information for the search algorithm. Also with the Travlr web application, I recreated the
	database to a relational database, and modified most of the website to be dynamically changed from
	the admin panel, and moves the application to a more Content-Management System (CMS) paradigm. I
	had previously made a CMS outside of this program, but building on the mistakes I made from back
	then helped me create this new CMS, and I was able to learn a few things while creating the new
	file.
</p>
<p>
	&emsp;I also added security measures in all of the applications. The Animal Shelter application
	only used a Base32 encoding previously for the password hash, which is not secure, and was changed
	to use bcrypt along with the Travlr application.
</p>
<p>
	&emsp;The following artifacts and their enhancements fit together to showcase my abilities by
	providing a rounded view of the field of computer science and software engineering. There are
	three categories of enhancements:
</p>
<ul class="list-disc list-inside ml-8">
	<li>Software Design and Engineering</li>
	<li>Algorithms and Data Structures</li>
	<li>Databases</li>
</ul>
<p>
	&emsp;There are three artifacts in total, with one of them spanning all three categories. The
	first is the Animal Shelter python library and web dashboard that displays a map of dogs up for
	adoption. The second is an embedded thermostat that uses state machines and hardware interrupts
	and timers to perform the actions of a user-set thermostat. The last project, which spans all
	three categories, is a commercial web application that facilitates the actions of a travel agency.
</p>
