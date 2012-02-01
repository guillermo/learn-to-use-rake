--author Guillermo Alvarez <guillermo.alvarez@wooga.net>
--heading Rake: The make way
--newpage Normal Structure
--heading Rake: The make way
We have a typical scenario:
---
.
├── Rakefile
└── public
    ├── css
    │   ├── admin.css
    │   ├── fonts.css
    │   ├── main.less
    │   └── reset.css
    ├── images
    │   ├── image1.png
    │   ├── image3.tiff
    │   └── nyan.tiff
    └── index.html

---
And we want to convert the tiff files into png
--newpage Before
--heading Refactor
Normally we do:
--beginoutput
task :convert_tiff_to_png do
  `convert public/images/nyan.tiff public/images/nyan.png` 
end
--endoutput
---
--beginoutput
task :convert_tiff_to_png do
  sh "convert public/images/nyan.tiff public/images/nyan.png"
end
--endoutput
---
--beginoutput
file "public/images/nyan.tiff" do
  sh "convert public/images/nyan.tiff public/images/nyan.png"
end
--endoutput
---
--beginoutput
file "public/images/nyan.tiff" => "public/images/nyan.png" do |t|
  sh "convert #{t.sournce} #{t.name}"
end
--endoutput
--newpage
--heading Rules
--beginoutput
rule '.png' => '.tiff' do |t|
  sh "convert #{t.sournce} #{t.name}"
end
--endoutput
---
--beginshelloutput
$ rake public/images/nyan.png
convert public/images/nyan.tiff public/images/nyan.png
--endshelloutput
---
--beginoutput
PNGFROMTIFF = FileList['public/images/**/*.tiff].ext('.png')

desc "Build the png images from tiff images"
task :build_images => PNGFROMTIFF
--endoutput
---
--beginshelloutput
$ rake build_images
convert public/images/image3.tiff public/images/image3.png
convert public/images/nyan.tiff public/images/nyan.png
--endshelloutput
--newpage
--heading Clean
--beginoutput
require 'rake/clean'

PNGFROMTIFF = FileList['public/images/**/*.tiff].ext('.png')

CLEAN.add PNGFROMTIFF
--endoutput
---
--beginshelloutput
$ rake clean
rm -r public/images/image3.png
rm -r public/images/nyan.png
--endshelloutput
---
--beginoutput
require 'rake/clean'

PNGFROMTIFF = FileList['public/images/**/*.tiff].ext('.png')

--boldon
CLOBBER.add PNGFROMTIFF
--boldoff
--endoutput
--newpage
--heading The same for the less files
--beginoutput
rule ".css" => '.less' do |t|
  sh "lessc #{t.source} > #{t.name}"
end

CSSFROMLESS = FileList['public/css/**/*.less'].ext('.css') 

desc "Build the css from the less files"
task :build_css => CSSFROMLESS

CLOBBER.add(CSSFROMLESS)
--endoutput

--newpage
--heading Grouping
We want to reference all the css+ the css that actually didn't exists:
---
--beginoutput
CSSFILES=FileList["public/css/**/*.css"]
CSSFILES.include CSSFROMLESS
--endoutput
---
So now we can generate the typical all.css
--beginoutput
file 'build/css/all.css' => CSSFILES do |t| 
  sh "cat #{CSSFILES} > #{t.name}"
end
--endoutput
---
But it fails because the build directory doesn't exists. Fixit:
---
--beginoutput
directory "build/css"
file 'build/css/all.css' => ['build/css', CSSFILES].flatten do |t| 
  sh "cat #{CSSFILES} > #{t.name}"
end
--endoutput
--newpage
--heading All

--beginoutput
directory "build/public/images"
IMAGES.each do |image|
  file "build/#{image}" => ["build/public/images", image] do |t|
    cp t.prerequisites[1], t.name
  end
end

task :all => [*IMAGES.pathmap("build/%p"),"build/css/all.css"]
--endoutput
--newpage
--beginshelloutput
$ rake all
mkdir -p build/public/images
cp public/images/image1.png build/public/images/image1.png
convert public/images/image3.tiff public/images/image3.png
cp public/images/image3.png build/public/images/image3.png
convert public/images/nyan.tiff public/images/nyan.png
cp public/images/nyan.png build/public/images/nyan.png
mkdir -p build/css
lessc public/css/main.less > public/css/main.css
cat public/css/admin.css public/css/fonts.css public/css/reset.css public/css/main.css > build/css/all.css
--endshelloutput
---
--beginshelloutput
$ rake all

--endshelloutput
--newpage
--beginshelloutput
$ echo '.all{display:none;}' > public/css/main.less  && rake all
lessc public/css/main.less > public/css/main.css
cat public/css/admin.css public/css/fonts.css public/css/main.css public/css/reset.css public/css/main.css > build/css/all.css
--endshelloutput

--newpage
--heading Nice things

* FileUtils is included, so you have all his methods by default
* FileList
* task for define tasks. multitask to define task that will run all the deps in different threads.
* pathmap
  %p — The complete path.
  %f — The base file name of the path, with its file extension, but without any directories.
  %n — The file name of the path without its file extension.
  %d — The directory list of the path.
  %x — The file extension of the path. An empty string if there is no extension.
  %X — Everything but the file extension.
  %s — The alternate file separater if defined, otherwise use the standard file separator.
  %% — A percent sign.


