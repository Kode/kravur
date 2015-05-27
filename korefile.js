var solution = new Solution('kravur');
var project = new Project('kravur');

solution.setCmd();

project.addExclude(".git/**");
project.addExclude("build/**");

project.addFile('Sources/**');

solution.addProject(project);

return solution;

