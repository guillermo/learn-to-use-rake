=begin

This is a sample **Rakefile** that output a site build in the output folder.

Remember that rake is allways run on the same directory where the __Rakefile__ is. So you don't need to worry.

You can read more about Rake in:

  * Tutorial with all the features included in this file: http://docs.rubyrake.org/tutorial/index.html
  * FileList documentation: http://rake.rubyforge.org/classes/Rake/FileList.html
  * Rdoc Documentation: http://rake.rubyforge.org/
  * pathmap arguments: http://rake.rubyforge.org/classes/String.html#M000017

=end

require 'rake/clean'
# Rake/clean give as to tasks: **clean** and **clobber**.
# **clean** is a dependency to **clobber**, so **clobber** will also run clean.
# You just need to do ``CLEAN.add`` or ``CLOBBER.add`` to append files that should be removed.
#
# In this example, will use **clean** to remove the __build__ directory, and **clobber** to remove all the generated files.


# PNG GENERATION
rule '.png' => '.tiff' do |t|
  sh "convert #{t.source} #{t.name}"
end

PNGFROMTIFF = FileList['public/images/**/*.tiff'].ext('.png') # Get all the tiff and change the extension to png
CLOBBER.add(PNGFROMTIFF)


# CSS GENERATION
rule ".css" => '.less' do |t|
  sh "lessc #{t.source} > #{t.name}"
end

CSSFROMLESS = FileList['public/css/**/*.less'].ext('.css')
CLOBBER.add(CSSFROMLESS)


# build/css/all.css
CSSFILES=FileList["public/css/**/*.css"]
CSSFILES.include CSSFROMLESS

directory "build/css"
file 'build/css/all.css' => ['build/css', CSSFILES].flatten do |t|
  sh "cat #{CSSFILES} > #{t.name}"
end

# build/images/*.png
IMAGES=FileList["public/images/**/*.png"]
IMAGES.include PNGFROMTIFF

directory "build/public/images"
IMAGES.each do |image|
  file "build/#{image}" => ["build/public/images", image] do |t|
    cp t.prerequisites[1], t.name
    # One useful case could be:
    # sh "optimezepng #{t.prerequisites[1]} #{t.name}"
  end
end

CLEAN.add "build" # clean will remove the build directory.
task :all => [*IMAGES.pathmap("build/%p"),"build/css/all.css"]

# To optimize build times, you can use **multitask**. It will execute all the dependencies in separete threads
multitask :all_threaded => [*IMAGES.pathmap("build/%p"),"build/css/all.css"]


