#!/usr/bin/env node

const { XMLParser, XMLBuilder, XMLValidator} = require("fast-xml-parser");
const fs = require("fs");

const opt = {
  ignoreAttributes: false
};

const text = fs.readFileSync("saves/arena.tmx", { encoding: "utf8", flag: "r" });
const xml = new XMLParser(opt).parse(text);

const width = parseInt(xml.map.layer["@_width"]);
const height = parseInt(xml.map.layer["@_height"]);

const data = xml.map.layer.data["#text"].split(",").map((x) => {
  if (x.trim() == "129") return 0;
  if (x.trim() == "130") return 1;
});

console.log(width, height);
console.log(data.join(" "));
