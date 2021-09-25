let project = new Project('New Project');

project.addFile('Sources/**');
project.addFile('Shaders/**');
project.setDebugDir('Deployment');

resolve(project);
