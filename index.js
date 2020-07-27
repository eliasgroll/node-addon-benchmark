const cpp = require("bindings")("node_addon_benchmark");
const cpuLimit = require("cpulimit");
const args = process.argv.slice(2);
const amount = Number.parseInt(args[0]);
const cpuPercentage = args[1];
const ram = args[2];

console.log(`${amount} invocations, ${cpuPercentage}% cpu, ${ram}mb ram`);

cpuLimit.createProcessFamily({
    limit: cpuPercentage,
    includeChildren: true,
    pid: 2324
}, () => {
    const test = () => { };
    let name;
    let start = 0;

    const log = (name, time) => {
        console.log(`${name}: ${Math.round((amount / time) * 1000)}/s`);
    }

    start = Date.now();
    for (let i = 0; i < amount; i++) {
        test();
    }
    log("Node.js invocation", Date.now() - start);

    start = Date.now();
    for (let i = 0; i < amount; i++) {
        cpp.request();
    }
    log("Node addon invocation", Date.now() - start);

    let counter = 0;
    const resolveSync = () => {
        counter++;
        if (counter == amount) {
            log("Node addon sync callback", Date.now() - start);
        }
    }
    start = Date.now();
    cpp.stream(amount, () => resolveSync());

    counter = 0;
    start = Date.now();
    const resolveAsync = () => {
        counter++;
        if (counter == amount) {
            log("Node addon async callback", Date.now() - start);
        }
    }
    cpp.streamThreadSafe(amount, () => resolveAsync());
});
