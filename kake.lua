solution = Solution.new("kravur")
project = Project.new("kravur")

solution:cmd()

project:addExclude(".git/**")
project:addExclude("build/**")

project:addFile("Sources/**")

solution:addProject(project)
