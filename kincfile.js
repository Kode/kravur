var project = new Project('kravur');

project.cmd = true;

project.addExclude(".git/**");
project.addExclude("build/**");

project.addFile('Sources/**');

resolve(project);
